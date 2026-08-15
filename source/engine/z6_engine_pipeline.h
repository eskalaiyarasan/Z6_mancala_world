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
#include <memory>
#include <string>
#include "movereq_schema.h"
#include "moveresp_schema.h"

// Extended Context to hold pipeline state data (msg and parsed request)
class Z6ExtendedPipelineContext : public Z6PipelineContext {
private:
    std::string msg_;
    Z6MoveReqJson move_req_;
    Z6BoardRespJson move_resp_;
public:
    Z6ExtendedPipelineContext(int fd, const std::string& msg)
        : Z6PipelineContext(fd), msg_(msg) {}

    std::string get_msg() const { return msg_; }
    Z6MoveReqJson& get_request() const { return move_req_;}
    Z6BoardRespJson& get_response() const { return move_resp_;}
    bool update_request(){move_req_.from_json(msg_); return true; }
};

// 1. Qualify Stage
class QualifyStage : public Z6PipelineStage {
public:
    std::unique_ptr<Z6PipelineContext> Execute(std::unique_ptr<Z6PipelineContext> context) override ;
};

// 2. Action Stage
class ActionStage : public Z6PipelineStage {
public:
    std::unique_ptr<Z6PipelineContext> Execute(std::unique_ptr<Z6PipelineContext> context) override ;
};

// 3. Update Stage
class UpdateStage : public Z6PipelineStage {
public:
    std::unique_ptr<Z6PipelineContext> Execute(std::unique_ptr<Z6PipelineContext> context) override ;
};
