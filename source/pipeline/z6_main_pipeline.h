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

#include <storage/z6_storage.h>
#include <pipeline/z6_pipeline.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

class Z6ValidationPipelineContext: public Z6PipelineContext{
    protected:
        std::string json_;
    public:
        Z6ValidationPipelineContext(int fd , std::string msg):Z6PipelineContext(fd),json_(msg){context_id=0;}
        std::string get() {return json_;}
};
class Z6WaitingPipelineContext: public Z6ValidationPipelineContext{
    private:
        std::vector<int> fd_list_;
        std::string uuid_str_;
        std::mutex mtx_;
    public:
        Z6WaitingPipelineContext(Z6ValidationPipelineContext *data);
        std::string get() { return uuid_str_;}

        bool addme(Z6ValidationPipelineContext *c);

        bool publish();
        int count();
        bool erase();
};
class Z6ValidationStage : public Z6PipelineStage {

public:
   Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};
class Z6WaitingStage : public Z6PipelineStage {
private:
    std::shared_ptr< Z6Storage<Z6ValidationPipelineContext> > local_;
public:
    Z6WaitingStage(std::shared_ptr< Z6Storage<Z6ValidationPipelineContext> > l):local_(l){}
   Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};
class Z6PublishStage : public Z6PipelineStage {

public:
   Z6PipelineContext*  Execute(Z6PipelineContext *context) override;
};
