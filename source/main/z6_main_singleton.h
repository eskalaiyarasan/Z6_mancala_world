/**
 * @file source/Z6Singleton/Z6Singleton.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#pragma once
#include <z6_singleton.h>
#include <z6_storage.h>
#include <z6_socket.h>
#include <z6_process.h>

#define MAIN_APP_SOCKET "/tmp/.z6_main.sock"
class Z6ChartNplayers: public Z6Singleton<Z6ChartNplayers>{
    friend class Z6Singleton<Z6ChartNplayers>;
    private :
        Z6ChartNplayers();
        Z6Storage<int> _store;
    public:
        int get(std::string name);
};

class Z6MainSocketServer: public  Z6Singleton<Z6MainSocketServer>{
    friend class Z6Singleton<Z6MainSocketServer>;
    private:
        Z6socket server;
        Z6MainSocketServer():server(std::string(MAIN_APP_SOCKET)){}
    public:
        Z6socket& get(){
            return server;
        }
};

class Z6ProcessManager: public  Z6Singleton<Z6Process>{
    friend class Z6Singleton<Z6Process>;
    private:
        Z6Process process;
    public:
        Z6Process& get(){
            return process;
        }
};
