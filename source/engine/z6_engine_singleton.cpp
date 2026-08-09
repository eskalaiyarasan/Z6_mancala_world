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
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <z6_engine_singleton.h>
#include <root_schema.h>
#include <game_schema.h>
#include <z6_algo_factory.h>
static bool readFileContent(const std::string& filename , std::string& fileContent){
    if (std::filesystem::exists(filename)) {
            std::ifstream file(filename);

            // Read everything into a single string via stringstream
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string fileContent = buffer.str();
        } else {
            std::cout << "File does not exist." << std::endl;
            return false;
        }
    return true;
}

bool Z6Root::init(std::string filename){
    std::string content;
    if(!readFileContent(filename,content)){
        return false;
    }
    Z6GameJson game;
    game.from_json(content);
    return jsonFactory_produce(game , root_);
}
void accept_note(int fd){
    auto data = Z6Root::getInstance().get().to_json()
    Z6EngineSocketServer::getInstance().SendResponse(fd , data);
}
