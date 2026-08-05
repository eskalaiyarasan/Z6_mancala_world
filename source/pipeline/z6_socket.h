#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <functional>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

class Z6socket {
private:
    std::string socket_path_;
    int server_fd_;
    int epoll_fd_;
    bool is_running_;
    //std::mutex map_mtx_;

    void setNonBlocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) flags = 0;
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    // Callback to send response back to socket client
    std::function<void(int, const char *)> response_callback_;
    void HandleAccept() {
        struct sockaddr_un cli_addr;
        socklen_t cli_len = sizeof(cli_addr);

        int client_fd = accept(server_fd_, reinterpret_cast<struct sockaddr*>(&cli_addr), &cli_len);
        if (client_fd < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                std::cerr << "[Error] accept failed: " << strerror(errno) << "\n";
            }
            return;
        }

        setNonBlocking(client_fd);

        // Add client fd to epoll watch list for reading
        struct epoll_event ev = {};
        ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP; // Edge-triggered read + peer close detection
        ev.data.fd = client_fd;

        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
            std::cerr << "[Error] Failed to add client fd to epoll: " << strerror(errno) << "\n";
            close(client_fd);
            return;
        }

        std::cout << "[Server] Accepted new client fd: " << client_fd << "\n";
    }

    void HandleRead(int client_fd) {
        char buf[4096];
        while (true) {
            ssize_t bytes_read = read(client_fd, buf, sizeof(buf) - 1);
            if (bytes_read < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    break; // Read all available data for now (Edge-triggered pattern)
                }
                // Real read error
                CloseClient(client_fd);
                break;
            } else if (bytes_read == 0) {
                // EOF - Client disconnected
                CloseClient(client_fd);
                break;
            } else {
                buf[bytes_read] = '\n';
                buf[bytes_read+1] = '\0';
                if(response_callback_){
                    response_callback_(client_fd, buf);
                }
            }
        }
    }

public:
    Z6socket(std::string socket_path)
        : socket_path_(std::move(socket_path)), server_fd_(-1), epoll_fd_(-1), is_running_(false), response_callback_(nullptr){

    }

    ~Z6socket() {
        Stop();
    }

    void Run() {
        unlink(socket_path_.c_str());

        // 1. Create Unix Stream Socket
        server_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (server_fd_ < 0) {
            throw std::runtime_error("Failed to create UNIX domain socket.");
        }

        setNonBlocking(server_fd_);

        struct sockaddr_un addr = {};
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

        if (bind(server_fd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
            close(server_fd_);
            throw std::runtime_error("Failed to bind UNIX domain socket.");
        }

        if (listen(server_fd_, 128) < 0) {
            close(server_fd_);
            throw std::runtime_error("Failed to listen on UNIX domain socket.");
        }

        // 2. Setup Epoll instance
        epoll_fd_ = epoll_create1(0);
        if (epoll_fd_ < 0) {
            close(server_fd_);
            throw std::runtime_error("Failed to create epoll descriptor.");
        }

        struct epoll_event ev = {};
        ev.events = EPOLLIN;
        ev.data.fd = server_fd_;

        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &ev) < 0) {
            close(server_fd_);
            close(epoll_fd_);
            throw std::runtime_error("Failed to add server socket to epoll.");
        }

        is_running_ = true;
        std::cout << "Raw Epoll UNIX Stream Server running on socket: " << socket_path_ << "\n";

        // 3. Event Loop
        const int MAX_EVENTS = 64;
        struct epoll_event events[MAX_EVENTS];

        while (is_running_) {
            int nfds = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
            if (nfds < 0) {
                if (errno == EINTR) continue;
                break;
            }

            for (int i = 0; i < nfds; ++i) {
                if (events[i].data.fd == server_fd_) {
                    // New connection request -> Perform accept()
                    HandleAccept();
                } else {
                    // Data available or hangup on existing client connection
                    int client_fd = events[i].data.fd;
                    if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                        CloseClient(client_fd);
                    } else if (events[i].events & EPOLLIN) {
                        HandleRead(client_fd);
                    }
                }
            }
        }
    }

    void Stop() {
        if (!is_running_) return;
        is_running_ = false;

        if (server_fd_ != -1) {
            close(server_fd_);
            server_fd_ = -1;
        }
        if (epoll_fd_ != -1) {
            close(epoll_fd_);
            epoll_fd_ = -1;
        }

        unlink(socket_path_.c_str());
        std::cout << "[Server] Stopped and cleaned up socket.\n";
    }

    // Public method to send a response directly to a client file descriptor
    void SendResponse(int client_fd, const std::string& payload) {
        std::string formatted = payload + "\n";
        ssize_t written = write(client_fd, formatted.data(), formatted.size());
        if (written < 0) {
            std::cerr << "[Error] Failed to write to client fd " << client_fd << ": " << strerror(errno) << "\n";
        }
    }

    // Explicitly close a client socket and remove it from epoll monitoring
    void CloseClient(int client_fd) {
        std::cout << "[Server] Closing client fd: " << client_fd << "\n";
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, client_fd, nullptr);
        close(client_fd);
    }
    void SetResponseCallback(std::function<void(int, const char *)> cb) {
        response_callback_ = cb;
    }
};
