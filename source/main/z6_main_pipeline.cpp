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
#include <z6_json_code.h>
#include <z6_main_pipeline.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <fstream>
#include <z6_uuid.h>
#include <thread>
#include <chrono>
using nlohmann::json_validator;
#define GAME_SCHEMA_JSON   "/schema/game.schema.json"

Z6WaitingPipelineContext::Z6WaitingPipelineContext(Z6ValidationPipelineContext *data):Z6ValidationPipelineContext(*data){
    context_id=2;
    uuid_str_=z6_uuid();
}
Z6PipelineContext*  Z6ValidationStage::Execute(Z6PipelineContext *context) {
    if(context->has_error>0){
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context;
    try{
        json_validator validator;
        std::ifstream schema_file(GAME_SCHEMA_JSON);
        if (!schema_file.is_open()) {
            std::cerr << "Failed to open root.schema.json\n";
            return 1;
        }
        // 2. Parse the schema file directly into a nlohmann::json object
        json schema_document = json::parse(schema_file);
        validator.set_root_schema(schema_document);
        validator.validate(c->get());
    }catch (const std::exception &e) {
            std::cerr << "Validation failed: " << e.what() << '\n';
            has_error = 1;
        }
    return context;
}

Z6PipelineContext*  Z6WaitingStage::Execute(Z6PipelineContext *context) {
    if(context->has_error>0){
        return context;
    }
    Z6ValidationPipelineContext *c = (Z6ValidationPipelineContext *) context;
    Z6GameJson  gamejson;
    bool  human=false;
    int level;
    std::string name;
    gamejson.from_json( c->get() );
    gamejson.getHuman(human);
    if(human){
        if( gamejson.getName(name) && (gamejson.getLevel(level) ){
            std::string key = name + key;
            auto found = local_.find(key);
            if ( found == nullptr){
                auto w = new Z6WaitingPipelineContext(c);
                local_.save(key, w);
                w->addme(c);
            }else{
                found->addme(c);
                if( Z6ChartNplayers::getInstance().get(name) >= found->count() ){
                    local_.erase(key)
                    return found;
                }else{
                    return nullptr;
                }
            }
            return nullptr;
        }
    }else{
       auto w = new Z6WaitingPipelineContext(c);
       return w;
    }
}

Z6PipelineContext*  Z6PublishStage::Execute(Z6PipelineContext *context) {

    if((context == nullptr )&&(context->has_error>0){
        return context;
    }
    context->publish();
    context->erase();
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
}

bool Z6WaitingPipelineContext::erase(){
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto fd : fd_list_){
        Z6MainSocketServer::getInstance().get().CloseClient(fd );
    }
    return true;
}

Z6ValidationPipelineContext(const Z6ValidationPipelineContext& others){
    json_ = others.get();
    context_id = 0;
    fd = others.get_client();
}

Z6PipelineContext* new_main_pipelineContext(int fd , const char *data){
    std:string data_str(data);
    auto ret = new  Z6ValidationPipelineContext(fd,data_str);
    return ret;
}
