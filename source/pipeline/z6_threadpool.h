/**
 * @file source/singleton/singleton.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */

#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include "z6_pipeline.h"

class Z6PrioritizedThreadPool {
private:
    std::vector<std::thread> standard_workers_;
    std::vector<std::thread> emergency_workers_;

    std::queue<std::shared_ptr<Z6PipelineContext>> standard_queue_;
    std::queue<std::shared_ptr<Z6PipelineContext>> emergency_queue_;

    std::mutex std_mtx_;
    std::mutex emg_mtx_;

    std::condition_variable std_cv_;
    std::condition_variable emg_cv_;

    bool stop_ = false;
    Pipeline pipeline_;



    void StandardWorkerLoop() ;

    void EmergencyWorkerLoop();

public:
    Z6PrioritizedThreadPool(size_t standard_threads, size_t reserved_emergency_threads, Pipeline pipeline);

    ~Z6PrioritizedThreadPool() ;

    void Enqueue(Z6PipelineContext *context);
};
