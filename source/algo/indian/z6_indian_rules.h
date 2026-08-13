/**
 * @file source/algo/z6_algo_base.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#pragma once
#include <z6_indian_rule_base.h>

class Z6IndianRuleSurya: Z6IndianRuleBase{
    public:
        Z6IndianRuleSurya(const Z6IndianRuleBase& base):Z6IndianRuleBase(base){

        }
        ~Z6IndianRuleSurya(){

        }
        Z6IndianRuleSurya(){}
        std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};
class Z6IndianRulePrithiv: Z6IndianRuleBase{
    public:
        Z6IndianRulePrithiv(const Z6IndianRuleBase& base):Z6IndianRuleBase(base){

        }
        ~Z6IndianRulePrithiv(){

        }
        Z6IndianRulePrithiv(){}
        std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};

class Z6IndianRuleSow: Z6IndianRuleBase{
    private:
        Z6RuleJson rule_;
    public:
    Z6IndianRuleSow(const Z6IndianRuleBase& base):Z6IndianRuleBase(base){
        rule_.setSowing(true);
    }
    ~Z6IndianRuleSow(){

    }
    std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};



class Z6IndianRuleCapture: Z6IndianRuleBase{
    private:
        Z6RuleJson rule_;
    protected:
        std::vector<Z6PitJson>::iterator  store0;
        std::vector<Z6PitJson>::iterator  store1;
        bool capture_flag;
        bool preset();
        bool update_store(int side , std::vector<Z6PitJson>::iterator);
    public:
    Z6IndianRuleCapture(const Z6IndianRuleBase& base):Z6IndianRuleBase(base),capture_flag(false){
        rule_.setCapture(true);
    }
    ~Z6IndianRuleCapture(){

    }
    std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};
class Z6IndianRulePasu:Z6IndianRuleCapture{
    private:
        Z6RuleJson rule_;
    protected:
        int pasu_;
        std::vector<Z6PitJson>::iterator  prev_;
        std::vector<Z6PitJson>::iterator  old_;
        bool preset();
    public:
    Z6IndianRulePasu(const int pasu , const Z6IndianRuleBase& base):Z6IndianRuleBase(base),pasu_(pasu){
        rule_.setPasu(true);
    }
    ~Z6IndianRulePasu(){

    }
    std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};
class Z6IndianRuleCaptureX2: Z6IndianRulePasu{
    private:
        Z6RuleJson rule_;
    public:
    Z6IndianRuleCaptureX2(const int pasu, const Z6IndianRuleBase& base):Z6IndianRulePasu(pasu,base){
        rule_.setDoubleCapture(true);
    }
    ~Z6IndianRuleCaptureX2(){

    }
    std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;
};


class Z6IndianRuleRoundReset: Z6IndianRulePasu{
    private:
        Z6RuleJson rule_;
        bool isGameEnded();
        bool refillPits();
    public:
    Z6IndianRuleRoundReset(const int pasu,const Z6IndianRuleBase& base):Z6IndianRulePasu(pasu,base){
        rule_.setRoundReset(true);
    }
    ~Z6IndianRuleRoundReset(){

    }
    std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) override;

};
