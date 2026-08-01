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
#include <z6_json_code.h>
using json = nlohmann::json;

// ==========================================
// 1. PIPELINE : Select - Preprocess -Action - Result
// ==========================================
class Z6PipelineContext{
    protected:
        int client_fd;
    public:
        int has_error=0;
        Z6PipelineContext(int fd):client_fd(fd){}
        int get_client(){return client_fd;}
        virtual Z6PipelineContext get()=0;
};

class Z6PipelineStage {
public:
    virtual ~Z6PipelineStage() = default;
    virtual Z6PipelineContext* Execute(Z6PipelineContext *context) = 0;
};

class Z6SelectionStage : public Z6PipelineStage {

public:
   Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};

class Z6PreprocessorStage : public Z6PipelineStage {
    public:
        Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};

class ActionStage : public Z6PipelineStage {
    public:
        Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};

class ResultStage : public Z6PipelineStage {
    public:
        Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};


class Pipeline {
private:
    std::vector<std::shared_ptr<Z6PipelineStage>> stages_;

public:
    void AddStage(std::shared_ptr<Z6PipelineStage> stage) {
        stages_.push_back(stage);
    }

    void Process( Z6PipelineContext  *context) {
        for (const auto& stage : stages_) {
            if (context->has_error) break;
            context = stage->Execute(context);
        }
    }
};
