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


#include <memory>
#include <type_traits>
#include <z6_storage.h>
#include <game_schema.h>
#include <z6_main_pipeline.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <z6_main_singleton.h>
#include <fstream>
#include <z6_utils.h>
#include <thread>
#include <chrono>

Z6WaitingPipelineContext::Z6WaitingPipelineContext(Z6ValidationPipelineContext *data):Z6ValidationPipelineContext(*data){
    context_id=2;
    uuid_str_=z6_uuid();
    client_fd = data->get_client();
    has_error = data->has_error;
}
std::unique_ptr<Z6PipelineContext>  Z6ValidationStage::Execute(std::unique_ptr<Z6PipelineContext>context) {
    std::cout<<" Z6ValidationStage::Execute\n";
    if(context->has_error>0){
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context.get();
    try{
        if( ! Z6GameJson::validate(c->get()) ){
            std::cout <<"Validation  Z6GameJson failed\n";
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
        std::cout<<" Z6WaitingStage::Execute   nullptr!!!\n";
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context.get();
    Z6GameJson gamejson;
    bool human = false;
    std::string name;
    gamejson.from_json( c->get() );
    gamejson.getHuman(human);

    if(human){
        if( processhuman(c, name)){
            return std::make_unique<Z6PublishPipelineContext>(c->get_client(), name);
        }else{
            return nullptr;
        }
    } else {
       auto w = std::make_unique<Z6WaitingPipelineContext>(c);
       w->addme(c);
       std::string key = w->get();
       local_->save(key, std::move(w));
       return std::make_unique<Z6PublishPipelineContext>(c->get_client(), key);
    }
    return context;
}

std::unique_ptr<Z6PipelineContext>  Z6PublishStage::Execute(std::unique_ptr<Z6PipelineContext>context) {

    if(context == nullptr ) {
        std::cout<<" Z6PublishStage::Execute nulptr!!!"<<"\n";
        return context;
    }
    if(context.get()->has_error>0){
        std::cout<<" Z6PublishStage::Execute "<<context.get()->has_error<<"\n";
    }

    Z6PublishPipelineContext *c = (Z6PublishPipelineContext *) context.get();
    auto found_ptr = local_->find(c->get_key());

    if ( !found_ptr ){
        std::cout<<"Z6PublishStage not able to find"<< c->get_key()<<"/n";
    }else{
        // Create the waiting context
        auto w = found_ptr->get();
        if(w){
            w->publish();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            w->erase();
        }
        local_->erase(c->get_key());
    }


    return context;
}

bool Z6WaitingPipelineContext::addme(Z6ValidationPipelineContext *c){
    Z6GameJson  gamejson;
    std::string name, player;
    gamejson.from_json( c->get() );
    gamejson.getPlayers(0, player );
    gamejson.getName(name);
    std::lock_guard<std::mutex> lock(mtx_);
    if( Z6ChartNplayers::getInstance().get(name) > fd_list_.size()){
        int fd = c->get_client();
        std::cout<<"Z6WaitingPipelineContext::addme " <<fd<<"\n";
        fd_list_.push_back(fd);
         Z6GameJson  gamejson;
         gamejson.from_json( json_ );
         std::vector<std::string> val;
         if(gamejson.getPlayersVector(val)) {
             val.push_back(player);
             gamejson.setPlayers(val);
         }
         json_ = gamejson.to_json();
        return true;
    } else{
        std::cout<<"Z6WaitingPipelineContext::addme Failed" <<Z6ChartNplayers::getInstance().get(name) <<"\n";
    }
    return false;
}
int Z6WaitingPipelineContext::count(){
    std::lock_guard<std::mutex> lock(mtx_);
     return fd_list_.size();
}

bool Z6WaitingPipelineContext::publish(){
    std::cout<<" Z6WaitingPipelineContext::publish\n";
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

bool Z6WaitingStage::processhuman(Z6ValidationPipelineContext *c , std::string &key1){
    Z6GameJson gamejson;
    bool human = false;
    int level;
    std::string name;
    gamejson.from_json( c->get() );
    gamejson.getHuman(human);
    if(!human){
        return false;
    }
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
            local_->save(key, std::move(w));
        } else {
            // FIX: Cast base pointer to the derived type to access addme() and count()
            Z6WaitingPipelineContext* waiting_ctx = found_ptr->get();

            if (waiting_ctx) {
                waiting_ctx->addme(c);
                if( Z6ChartNplayers::getInstance().get(name) <= waiting_ctx->count() ){
                    //local_->erase(key);
                    std::cout<<"Z6WaitingStage::processhuman " <<waiting_ctx->count()<<"\n";
                    key1 = key;
                    return true;
                } else {
                    // Keep it in storage. The item is updated in place.
                    std::cout<<"Z6WaitingStage::processhuman "<<Z6ChartNplayers::getInstance().get(name) <<"<=>" <<waiting_ctx->count()<<"\n";
                    return false;
                }
            }
        }
        return false;
    }
}

Z6PublishPipelineContext::Z6PublishPipelineContext(int fd, std::string &key):
    Z6PipelineContext(fd),key_(key){

    }
