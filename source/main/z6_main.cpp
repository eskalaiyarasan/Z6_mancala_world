/**
 * @file source/main/main.h
 * @author  kalaiyarasan S
 * @date 2026-08-02
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */

#include "z6_threadpool.h"
#include "z6_main_pipeline.h"
#include "z6_main_singleton.h"
#include <stdexcept>
#include <memory>
#include <csignal>
#include <z6_utils.h>

int main(){
    Z6NameSpace::nSocketClient=64;
    signal(SIGPIPE, SIG_IGN);
    try {
        Z6ChartNplayers::getInstance();
        Z6ProcessManager::getInstance();
        auto main_hash =std::make_shared< Z6Storage< std::unique_ptr <Z6WaitingPipelineContext> > >();
        Z6Pipeline pipeline;
        pipeline.AddStage(std::make_shared<Z6ValidationStage>());
        pipeline.AddStage(std::make_shared<Z6WaitingStage>(main_hash));
        pipeline.AddStage(std::make_shared<Z6PublishStage>(main_hash));

        // Create thread pool: 4 standard workers, 1 reserved emergency worker
        auto thread_pool = std::make_shared<Z6PrioritizedThreadPool>(10, 1, pipeline);
        auto server = Z6MainSocketServer::getInstance().get();
        auto callback = [&thread_pool](int fd, const char* data) {
                thread_pool->Enqueue(new_main_pipelineContext(fd ,data));
            };
        server.SetResponseCallback(callback);
        server.Run();
    }catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
