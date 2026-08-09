/**
 * @file source/engine/z6_engine_singleton.cpp
 * @author  kalaiyarasan S
 * @date 2026-08-06
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#pragma once
#include <iostream>
#include <memory>
#include <z6_singleton.h>
#include <root_schema.h>
#include <z6_socket.h>
#include <root_schema.h>
#include <z6_pipeline.h>
#include <z6_algo_base.h>

namespace Z6EngineSpace {
struct AppConfig {
    std::string filename = "";
    std::string id = 0;
    bool helpRequested = false;
};
extern AppConfig config;
}
class Z6EngineSocketServer: public  Z6Singleton<Z6EngineSocketServer>{
    friend class Z6Singleton<Z6EngineSocketServer>;
    private:
        Z6socket server;
        Z6EngineSocketServer():server("/tmp/"+Z6EngineSpace::config.id+".sock"){}
    public:
        Z6socket& get(){
            return server;
        }
};

class Z6Root: public Z6Singleton<Z6Root>{
    friend class Z6Singleton<Z6Root>;
    private :
        Z6RootJson root_;
        Z6Root();
    public:
       Z6Root& get()const { return root_;};
       Z6Root getCopy()const { return root_;}
       bool init(std::string filename);
};

class Z6Engine: public  Z6Singleton<Z6Engine>{
    friend class Z6Singleton<Z6Engine>;
    private:
        std::unique_ptr<Z6Algo> algo_;
        Z6Engine(){}
    public:
        Z6Algo* get(){ return algo_.get();}
        void set( std::unique_ptr<Z6Algo> &algo) {
            algo_ = std::move(algo);
        }
}

std::unique_ptr<Z6PipelineContext> new_engine_pipelineContext(int fd , const char *data);
void accept_note(int fd);
