/**
 * @file source/engine/z6_engine_main.cpp
 * @author  kalaiyarasan S
 * @date 2026-08-06
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#include<iostream>
#include "z6_threadpool.h"
#include <z6_engine_singleton.h>
#include <string>
#include <unistd.h> // for getopt, optarg, optind
#include <stdexcept>
#include <memory>
#include <csignal>

// Function that handles argc and argv processing
bool parseArguments(int argc, char* argv[]) {
    // Reset optind to 1 in case getopt is called multiple times in a program
    optind = 1;

    int opt;
    while ((opt = getopt(argc, argv, "hf:i:")) != -1) {
        switch (opt) {
            case 'h':
                Z6EngineSpace::config.helpRequested = true;
                break;
            case 'f':
                Z6EngineSpace::config.filename = optarg;
                break;
            case 'c':
                Z6EngineSpace::config.id = optarg;
                break;
            default:
                return false; // Triggers usage display
        }
    }
    return true;
}

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [-h] [-f filename] [-i uuid] \n";
}
namespace Z6EngineSpace{
    AppConfig config;
}
int main(int argc, char *argv[]){
    signal(SIGPIPE, SIG_IGN);
    try {
        // Pass argc and argv to our custom function
        if (!parseArguments(argc, argv)) {
            printUsage(argv[0]);
            return 1;
        }

        if (Z6EngineSpace::config.helpRequested) {
            printUsage(argv[0]);
            return 0;
        }
        Z6EngineSocketServer::getInstance().init(Z6EngineSpace::config.filename);
        auto server = Z6EngineSocketServer::getInstance().get();
        server.SetAcceptCallback(accept_note);

        auto thread_pool = std::make_shared<Z6PrioritizedThreadPool>(1, 0, pipeline);
        auto callback = [&thread_pool](int fd, const char* data) {
                thread_pool->Enqueue(new_engine_pipelineContext(fd ,data));
            };
        server.SetResponseCallback(callback);
        server.Run();


    }catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }
}
