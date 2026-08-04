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

#include "z6_threadpool.h"

void Z6PrioritizedThreadPool::StandardWorkerLoop() {
    while (true) {
        Z6PipelineContext *context;
        {
            std::unique_lock<std::mutex> lock(std_mtx_);
            // Zero spurious wakeup loop predicate
            std_cv_.wait(lock, [this] { return stop_ || !standard_queue_.empty(); });

            if (stop_ && standard_queue_.empty()) return;

            context = standard_queue_.front();
            standard_queue_.pop();
            pipeline_.Process(context);
        }

    }
}

void Z6PrioritizedThreadPool::EmergencyWorkerLoop() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(emg_mtx_);
            // Dedicated emergency thread stays idle until signaled
            emg_cv_.wait(lock, [this] { return stop_ || !emergency_queue_.empty(); });

            if (stop_ && emergency_queue_.empty()) return;

            auto context = emergency_queue_.front();
            emergency_queue_.pop();
            pipeline_.Process(context);
        }

    }
}

Z6PrioritizedThreadPool::Z6PrioritizedThreadPool(size_t standard_threads, size_t reserved_emergency_threads, Pipeline pipeline)
    : pipeline_(std::move(pipeline)) {

    // Start standard workers
    for (size_t i = 0; i < standard_threads; ++i) {
        standard_workers_.emplace_back(&Z6PrioritizedThreadPool::StandardWorkerLoop, this);
    }

    // Start reserved critical/emergency workers (always idle until emergency task arrives)
    for (size_t i = 0; i < reserved_emergency_threads; ++i) {
        emergency_workers_.emplace_back(&Z6PrioritizedThreadPool::EmergencyWorkerLoop, this);
    }
}

Z6PrioritizedThreadPool::~Z6PrioritizedThreadPool() {
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


void Z6PrioritizedThreadPool::Enqueue(std::unique_ptr<Z6PipelineContext> context) {
    // Initial quick parse check to direct traffic before full pipeline stage execution
    if(context == nullptr){return;}

    try {
    if (context->is_emergency) {
        {
            std::lock_guard<std::mutex> lock(emg_mtx_);
            emergency_queue_.push(std::move(context));
        }
        emg_cv_.notify_one(); // Wake up reserved critical worker
    } else {
        {
            std::lock_guard<std::mutex> lock(std_mtx_);
            standard_queue_.push(std::move(context));
        }
        std_cv_.notify_one(); // Wake up standard worker
    }
}
