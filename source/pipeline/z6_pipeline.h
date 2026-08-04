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
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <cstring>
#include <nlohmann/json.hpp>

#include <z6_storage.h>

using json = nlohmann::json;

// ==========================================
// 1. PIPELINE : Select - Preprocess -Action - Result
// ==========================================
class Z6PipelineContext{
    protected:
        int context_id;
        int client_fd;
    public:
        int has_error;
        int is_emergency;
        Z6PipelineContext(int fd):client_fd(fd),context_id(-1), has_error(0){}
        int get_client(){return client_fd;}
        virtual ~Z6PipelineContext()=0;
};

class Z6PipelineStage {
public:
    virtual ~Z6PipelineStage() = default;
    virtual std::unique_ptr<Z6PipelineContext> Execute(std::unique_ptr<Z6PipelineContext >context) = 0;
};

class Z6Pipeline {
private:
    std::vector<std::shared_ptr<Z6PipelineStage>> stages_;

public:
    void AddStage(std::shared_ptr<Z6PipelineStage> stage) {
        stages_.push_back(stage);
    }

    void Process( std::unique_ptr<Z6PipelineContext> context) {
        for (const auto& stage : stages_) {
            context = stage->Execute(std::move(context)) ;
        }
    }
};
