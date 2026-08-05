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

#include <z6_storage.h>
#include <z6_pipeline.h>
#include <string>
#include <memory>
#include <mutex>
#include <cstring>
#include <string>


class Z6ValidationPipelineContext: public Z6PipelineContext{
    protected:
        std::string json_;
    public:
        Z6ValidationPipelineContext(int fd ,const std::string &msg):Z6PipelineContext(fd),json_(msg){context_id=0;}
        Z6ValidationPipelineContext(const Z6ValidationPipelineContext& others);
        std::string get() const {return json_;}
        ~Z6ValidationPipelineContext(){}
};
class Z6WaitingPipelineContext: public Z6ValidationPipelineContext{
    private:
        std::vector<int> fd_list_;
        std::string uuid_str_;
        std::mutex mtx_;
    public:
        Z6WaitingPipelineContext(Z6ValidationPipelineContext *data);
        std::string get() const{ return uuid_str_;}

        bool addme(Z6ValidationPipelineContext *c);
        bool publish();
        int count();
        bool erase();
        ~Z6WaitingPipelineContext(){}
};
class Z6PublishPipelineContext: public Z6PipelineContext{
    private:
        std::string key_;
    public:
        Z6PublishPipelineContext(int fd ,std::string &k);
        std::string get_key(){return key_;}
        ~Z6PublishPipelineContext(){}
};
class Z6ValidationStage : public Z6PipelineStage {

public:
   std::unique_ptr<Z6PipelineContext>  Execute(std::unique_ptr<Z6PipelineContext>context) override;
};
class Z6WaitingStage : public Z6PipelineStage {
private:
    std::shared_ptr< Z6Storage<std::unique_ptr<Z6WaitingPipelineContext> > > local_;
public:
    Z6WaitingStage(std::shared_ptr< Z6Storage<std::unique_ptr<Z6WaitingPipelineContext> > > st):local_(st){}
   std::unique_ptr<Z6PipelineContext>  Execute(std::unique_ptr<Z6PipelineContext>context) override;
   bool processhuman(Z6ValidationPipelineContext *c , std::string &key1);
};
class Z6PublishStage : public Z6PipelineStage {
private:
        std::shared_ptr< Z6Storage<std::unique_ptr<Z6WaitingPipelineContext> > > local_;
public:
    Z6PublishStage(std::shared_ptr< Z6Storage<std::unique_ptr<Z6WaitingPipelineContext> > > st):local_(st){}
   std::unique_ptr<Z6PipelineContext>  Execute(std::unique_ptr<Z6PipelineContext>context) override;
};

std::unique_ptr<Z6PipelineContext> new_main_pipelineContext(int fd , const char *data);
