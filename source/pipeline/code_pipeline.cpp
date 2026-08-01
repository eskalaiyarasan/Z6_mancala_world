#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ==========================================
// 1. DOMAIN MODELS & CONTEXT
// ==========================================

struct RequestContext {
    std::string raw_input;
    json request_json;
    std::string client_id;
    nlohmann::json* root_object = nullptr;
    std::string response_payload;
    bool has_error = false;
    bool is_emergency = false;
    int client_fd = -1;
};

// ==========================================
// 2. INTERFACES (DIP & ISP)
// ==========================================

class IRouter {
public:
    virtual ~IRouter() = default;
    virtual json* FindRootObject(const std::string& id) = 0;
};

class IPipelineStage {
public:
    virtual ~IPipelineStage() = default;
    virtual void Execute(std::shared_ptr<RequestContext> context) = 0;
};

// ==========================================
// 3. CONCRETE ROUTER
// ==========================================

class HashTableRouter : public IRouter {
private:
    std::unordered_map<std::string, json> store_;
    std::mutex mtx_;

public:
    HashTableRouter() {
        store_["ID-101"] = {{"ID", "ID-101"}, {"status", "ACTIVE"}, {"counter", 10}};
        store_["ID-202"] = {{"ID", "ID-202"}, {"status", "PENDING"}, {"counter", 20}};
    }

    json* FindRootObject(const std::string& id) override {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = store_.find(id);
        if (it != store_.end()) {
            return &(it->second);
        }
        return nullptr;
    }
};

// ==========================================
// 4. PIPELINE STAGES
// ==========================================

class SelectionStage : public IPipelineStage {
private:
    std::shared_ptr<IRouter> router_;

public:
    explicit SelectionStage(std::shared_ptr<IRouter> router) : router_(router) {}

    void Execute(std::shared_ptr<RequestContext> context) override {
        try {
            context->request_json = json::parse(context->raw_input);
            
            if (!context->request_json.contains("ID")) {
                throw std::runtime_error("Missing 'ID' field in JSON payload.");
            }

            // Check if emergency flag is passed in JSON
            if (context->request_json.contains("emergency") && context->request_json["emergency"].get<bool>()) {
                context->is_emergency = true;
            }

            context->client_id = context->request_json["ID"].get<std::string>();
            context->root_object = router_->FindRootObject(context->client_id);

            if (!context->root_object) {
                throw std::runtime_error("Root object not found for ID: " + context->client_id);
            }
        } catch (const std::exception& e) {
            context->has_error = true;
            context->response_payload = json{{"error", e.what()}}.dump();
        }
    }
};

class PreprocessorStage : public IPipelineStage {
public:
    void Execute(std::shared_ptr<RequestContext> context) override {
        if (context->has_error) return;
        if (!context->request_json.contains("action_type")) {
            context->has_error = true;
            context->response_payload = json{{"error", "Missing 'action_type' specification."}}.dump();
        }
    }
};

class ActionStage : public IPipelineStage {
public:
    void Execute(std::shared_ptr<RequestContext> context) override {
        if (context->has_error) return;

        std::string action_type = context->request_json["action_type"];
        if (action_type == "INCREMENT") {
            (*context->root_object)["counter"] = (*context->root_object)["counter"].get<int>() + 1;
            (*context->root_object)["status"] = context->is_emergency ? "EMERGENCY_UPDATED" : "UPDATED";
        } else {
            (*context->root_object)["status"] = "PROCESSED";
        }
    }
};

class ResultStage : public IPipelineStage {
public:
    void Execute(std::shared_ptr<RequestContext> context) override {
        if (context->has_error) return;

        json response;
        response["status"] = "success";
        response["emergency"] = context->is_emergency;
        response["data"] = *(context->root_object);
        
        context->response_payload = response.dump();
    }
};

class Pipeline {
private:
    std::vector<std::shared_ptr<IPipelineStage>> stages_;

public:
    void AddStage(std::shared_ptr<IPipelineStage> stage) {
        stages_.push_back(stage);
    }

    void Process(std::shared_ptr<RequestContext> context) {
        for (const auto& stage : stages_) {
            stage->Execute(context);
            if (context->has_error) break;
        }
    }
};

// ==========================================
// 5. THREAD POOL WITH RESERVED EMERGENCY WORKERS
// ==========================================

class PrioritizedThreadPool {
private:
    std::vector<std::thread> standard_workers_;
    std::vector<std::thread> emergency_workers_;

    std::queue<std::shared_ptr<RequestContext>> standard_queue_;
    std::queue<std::shared_ptr<RequestContext>> emergency_queue_;

    std::mutex std_mtx_;
    std::mutex emg_mtx_;
    
    std::condition_variable std_cv_;
    std::condition_variable emg_cv_;

    bool stop_ = false;
    Pipeline pipeline_;
    
    // Callback to send response back to socket client
    std::function<void(int, const std::string&)> response_callback_;

    void StandardWorkerLoop() {
        while (true) {
            std::shared_ptr<RequestContext> context;
            {
                std::unique_lock<std::mutex> lock(std_mtx_);
                // Zero spurious wakeup loop predicate
                std_cv_.wait(lock, [this] { return stop_ || !standard_queue_.empty(); });
                
                if (stop_ && standard_queue_.empty()) return;

                context = standard_queue_.front();
                standard_queue_.pop();
            }

            // Execute pipeline service
            pipeline_.Process(context);
            if (response_callback_) {
                response_callback_(context->client_fd, context->response_payload);
            }
        }
    }

    void EmergencyWorkerLoop() {
        while (true) {
            std::shared_ptr<RequestContext> context;
            {
                std::unique_lock<std::mutex> lock(emg_mtx_);
                // Dedicated emergency thread stays idle until signaled
                emg_cv_.wait(lock, [this] { return stop_ || !emergency_queue_.empty(); });
                
                if (stop_ && emergency_queue_.empty()) return;

                context = emergency_queue_.front();
                emergency_queue_.pop();
            }

            std::cout << "[EMERGENCY THREAD] Processing high-priority task for ID: " << context->client_id << "\n";
            pipeline_.Process(context);
            if (response_callback_) {
                response_callback_(context->client_fd, context->response_payload);
            }
        }
    }

public:
    PrioritizedThreadPool(size_t standard_threads, size_t reserved_emergency_threads, Pipeline pipeline)
        : pipeline_(std::move(pipeline)) {
        
        // Start standard workers
        for (size_t i = 0; i < standard_threads; ++i) {
            standard_workers_.emplace_back(&PrioritizedThreadPool::StandardWorkerLoop, this);
        }

        // Start reserved critical/emergency workers (always idle until emergency task arrives)
        for (size_t i = 0; i < reserved_emergency_threads; ++i) {
            emergency_workers_.emplace_back(&PrioritizedThreadPool::EmergencyWorkerLoop, this);
        }
    }

    ~PrioritizedThreadPool() {
        {
            std::lock_guard<std::mutex> lock1(std_mtx_);
            std::lock_guard<std::mutex> lock2(emg_mtx_);
            stop_ = true;
        }
        std_cv_.notify_all();
        emg_cv_.notify_all();

        for (auto& t : standard_workers_) if (t.joinable()) t.join();
        for (auto& t : emergency_workers_) if (t.joinable()) t.join();
    }

    void SetResponseCallback(std::function<void(int, const std::string&)> cb) {
        response_callback_ = cb;
    }

    void Enqueue(std::shared_ptr<RequestContext> context) {
        // Initial quick parse check to direct traffic before full pipeline stage execution
        try {
            auto j = json::parse(context->raw_input);
            if (j.contains("emergency") && j["emergency"].get<bool>()) {
                context->is_emergency = true;
            }
        } catch (...) {
            // Let the selection stage handle syntax errors
        }

        if (context->is_emergency) {
            {
                std::lock_guard<std::mutex> lock(emg_mtx_);
                emergency_queue_.push(context);
            }
            emg_cv_.notify_one(); // Wake up reserved critical worker
        } else {
            {
                std::lock_guard<std::mutex> lock(std_mtx_);
                standard_queue_.push(context);
            }
            std_cv_.notify_one(); // Wake up standard worker
        }
    }
};

// ==========================================
// 6. LIBEVENT UNIX DOMAIN SOCKET SERVER
// ==========================================

class UnixDomainSocketServer {
private:
    std::string socket_path_;
    struct event_base* base_;
    int server_fd_;
    struct event* listener_event_;
    std::shared_ptr<PrioritizedThreadPool> thread_pool_;
    std::unordered_map<int, struct bufferevent*> client_bev_map_;
    std::mutex map_mtx_;

    static void ReadCallback(struct bufferevent* bev, void* ctx) {
        auto* server = static_cast<UnixDomainSocketServer*>(ctx);
        struct evbuffer* input = bufferevent_get_input(bev);
        size_t len = evbuffer_get_length(input);
        
        std::vector<char> buf(len + 1);
        evbuffer_remove(input, buf.data(), len);
        buf[len] = '\0';

        int client_fd = bufferevent_getfd(bev);

        auto context = std::make_shared<RequestContext>();
        context->raw_input = std::string(buf.data());
        context->client_fd = client_fd;

        // Offload to multi-threaded pipeline pool
        server->thread_pool_->Enqueue(context);
    }

    static void EventCallback(struct bufferevent* bev, short events, void* ctx) {
        auto* server = static_cast<UnixDomainSocketServer*>(ctx);
        int fd = bufferevent_getfd(bev);
        
        if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
            server->RemoveClient(fd);
            bufferevent_free(bev);
        }
    }

    static void AcceptCallback(evutil_socket_t listener, short event, void* arg) {
        auto* server = static_cast<UnixDomainSocketServer*>(arg);
        struct sockaddr_un cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        
        int client_fd = accept(listener, (struct sockaddr*)&cli_addr, &cli_len);
        if (client_fd < 0) return;

        struct bufferevent* bev = bufferevent_socket_new(server->base_, client_fd, BEV_OPT_CLOSE_ON_FREE);
        server->AddClient(client_fd, bev);
        
        bufferevent_setcb(bev, ReadCallback, nullptr, EventCallback, server);
        bufferevent_enable(bev, EV_READ | EV_WRITE);
    }

    void AddClient(int fd, struct bufferevent* bev) {
        std::lock_guard<std::mutex> lock(map_mtx_);
        client_bev_map_[fd] = bev;
    }

    void RemoveClient(int fd) {
        std::lock_guard<std::mutex> lock(map_mtx_);
        client_bev_map_.erase(fd);
    }

    void SendResponse(int client_fd, const std::string& payload) {
        std::lock_guard<std::mutex> lock(map_mtx_);
        auto it = client_bev_map_.find(client_fd);
        if (it != client_bev_map_.end()) {
            struct evbuffer* output = bufferevent_get_output(it->second);
            evbuffer_add(output, payload.data(), payload.size());
            evbuffer_add(output, "\n", 1);
            
            // Multicast simulation to all connected clients
            for (auto& pair : client_bev_map_) {
                struct evbuffer* out = bufferevent_get_output(pair.second);
                evbuffer_add(out, ("[Multicast] " + payload + "\n").data(), payload.size() + 13);
            }
        }
    }

public:
    UnixDomainSocketServer(std::string socket_path, std::shared_ptr<PrioritizedThreadPool> pool)
        : socket_path_(std::move(socket_path)), thread_pool_(pool), base_(nullptr), server_fd_(-1), listener_event_(nullptr) {
        
        thread_pool_->SetResponseCallback([this](int fd, const std::string& res) {
            this->SendResponse(fd, res);
        });
    }

    ~UnixDomainSocketServer() {
        if (listener_event_) event_free(listener_event_);
        if (server_fd_ != -1) close(server_fd_);
        if (base_) event_base_free(base_);
        unlink(socket_path_.c_str());
    }

    void Run() {
        unlink(socket_path_.c_str());
        base_ = event_base_new();
        if (!base_) throw std::runtime_error("Failed to create libevent base.");

        server_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (server_fd_ < 0) throw std::runtime_error("Failed to create UNIX domain socket.");

        struct sockaddr_un addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

        if (bind(server_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            throw std::runtime_error("Failed to bind UNIX domain socket.");
        }

        if (listen(server_fd_, 128) < 0) {
            throw std::runtime_error("Failed to listen on UNIX domain socket.");
        }

        evutil_make_socket_nonblocking(server_fd_);
        listener_event_ = event_new(base_, server_fd_, EV_READ | EV_PERSIST, AcceptCallback, this);
        event_add(listener_event_, nullptr);

        std::cout << "Prioritized Pipeline Server running on UNIX socket: " << socket_path_ << "\n";
        event_base_dispatch(base_);
    }
};

// ==========================================
// 7. ENTRY POINT
// ==========================================

int main() {
    try {
        auto router = std::make_shared<HashTableRouter>();

        Pipeline pipeline;
        pipeline.AddStage(std::make_shared<SelectionStage>(router));
        pipeline.AddStage(std::make_shared<PreprocessorStage>());
        pipeline.AddStage(std::make_shared<ActionStage>());
        pipeline.AddStage(std::make_shared<ResultStage>());

        // Create thread pool: 4 standard workers, 1 reserved emergency worker
        auto thread_pool = std::make_shared<PrioritizedThreadPool>(4, 1, pipeline);

        UnixDomainSocketServer server("/tmp/pipeline_app.sock", thread_pool);
        server.Run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}