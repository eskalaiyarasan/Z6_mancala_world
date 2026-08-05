/**
 * @file source/main/z6_main_pipeline.cpp
 * @author  kalaiyarasan S
 * @date 2026-08-02
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */


#include <z6_storage.h>
#include <game_schema.h>
#include <z6_main_pipeline.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <z6_main_singleton.h>
#include <fstream>
#include <z6_uuid.h>

Z6WaitingPipelineContext::Z6WaitingPipelineContext(Z6ValidationPipelineContext *data):Z6ValidationPipelineContext(*data){
    context_id=2;
    uuid_str_=z6_uuid();
    client_fd = data->get_client();
    has_error = data->has_error;
}
std::unique_ptr<Z6PipelineContext>  Z6ValidationStage::Execute(std::unique_ptr<Z6PipelineContext>context) {
    if(context->has_error>0){
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context.get();
    try{
        if( ! Z6GameJson::validate(c->get()) ){
            c->has_error = 1;
        }
    }catch (const std::exception &e) {
            std::cerr << "Validation failed: " << e.what() << '\n';
            c->has_error = 1;
        }
    return context;
}

// source/main/z6_main_pipeline.cpp
std::unique_ptr<Z6PipelineContext> Z6WaitingStage::Execute(std::unique_ptr<Z6PipelineContext> context) {
    if(context->has_error > 0){
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context.get();
    Z6GameJson gamejson;
    bool human = false;
    int level;
    std::string name;
    gamejson.from_json( c->get() );
    gamejson.getHuman(human);

    if(human){
        if( gamejson.getName(name) && gamejson.getLevel(level) ){
            // FIX: Avoid reusing 'key' during definition
            std::string key = name + "_room";

            // 'found' is now a pointer to std::unique_ptr<Z6ValidationPipelineContext>
            auto found_ptr = local_->find(key);

            if ( !found_ptr ){
                // Create the waiting context
                auto w = std::make_unique<Z6WaitingPipelineContext>(c);
                w->addme(c);

                // FIX: Upcast the smart pointer cleanly to Base type for storage
                std::unique_ptr<Z6ValidationPipelineContext> base_w = std::move(w);
                local_->save(key, std::move(base_w));
            } else {
                // FIX: Cast base pointer to the derived type to access addme() and count()
                Z6WaitingPipelineContext* waiting_ctx = dynamic_cast<Z6WaitingPipelineContext*>(found_ptr->get());

                if (waiting_ctx) {
                    waiting_ctx->addme(c);
                    if( Z6ChartNplayers::getInstance().get(name) >= waiting_ctx->count() ){
                        local_->erase(key);
                        // FIX: Move the smart pointer out of the storage map to return it
                        return std::move(*found_ptr);
                    } else {
                        // Keep it in storage. The item is updated in place.
                        return nullptr;
                    }
                }
            }
            return nullptr;
        }
    } else {
       auto w = std::make_unique<Z6WaitingPipelineContext>(c);
       w->addme(c);
       return w;
    }
    return context;
}

std::unique_ptr<Z6PipelineContext>  Z6PublishStage::Execute(std::unique_ptr<Z6PipelineContext>context) {

    if((context == nullptr ) || (context.get()->has_error>0)){
        return context;
    }
    Z6WaitingPipelineContext *c = (Z6WaitingPipelineContext *) context.get();
    c->publish();
    c->erase();
    return context;
}

bool Z6WaitingPipelineContext::addme(Z6ValidationPipelineContext *c){
    Z6GameJson  gamejson;
    std::string name, player;
    gamejson.from_json( c->get() );
    gamejson.getPlayers(0, player );
    gamejson.getName(name);
    std::lock_guard<std::mutex> lock(mtx_);
    if( Z6ChartNplayers::getInstance().get(name) < fd_list_.size()){
        int fd = c->get_client();
        fd_list_.push_back(fd);
         Z6GameJson  gamejson;
         gamejson.from_json( json_ );
         std::vector<std::string> val;
         gamejson.getPlayersVector(val);
         val.push_back(player);
         gamejson.setPlayers(val);
         json_ = gamejson.to_json();
        return true;
    }
    return false;
}
int Z6WaitingPipelineContext::count(){
    std::lock_guard<std::mutex> lock(mtx_);
     return fd_list_.size();
}

bool Z6WaitingPipelineContext::publish(){
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto fd : fd_list_){
        Z6MainSocketServer::getInstance().get().SendResponse(fd, uuid_str_);
    }
    return true;
}

bool Z6WaitingPipelineContext::erase(){
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto fd : fd_list_){
        Z6MainSocketServer::getInstance().get().CloseClient(fd );
    }
    return true;
}

Z6ValidationPipelineContext::Z6ValidationPipelineContext(const Z6ValidationPipelineContext& others):
    Z6PipelineContext(others.get_client()), json_(others.get()){
    context_id = 0;
}

std::unique_ptr<Z6PipelineContext> new_main_pipelineContext(int fd , const char *data){
    std::string data_str(data);

    return std::make_unique<Z6ValidationPipelineContext>(fd,data_str);
}
