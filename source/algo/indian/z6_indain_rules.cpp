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
#include <string>
#include <z6_indian_rules.h>
#include <z6_utils.h>

std::vector<Z6PitJson>::iterator Z6IndianRuleSurya::step(std::vector<Z6PitJson>::iterator now) {
        std::string pit_type;
        std::vector<Z6PitJson>::iterator next = now;
        next ++;
        bool done=false;
        bool first = false;
        while(!done){
            if(next == pits_.end()){
                next = pits_.begin();
                if(first){
                    error_=Z6IndianErrorEnums::ERROR_SOWING_NOT_POSSIBLE;
                    done = true;
                    stop_ = true;
                }else{
                    first = true;
                }
            }
            if(next->getType(pit_type) && (! compareStr(pit_type, "STORE") ) ){
                bool enable;
                if(next->getEnable(enable) && enable){
                    return next;
                } else{
                    next++;
                }

            } else{
                next ++;
            }
        }
    return pits_.end();
}

std::vector<Z6PitJson>::iterator Z6IndianRulePrithiv::step(std::vector<Z6PitJson>::iterator now) {
        std::string pit_type;
        std::vector<Z6PitJson>::iterator next = now;
        next --;
        bool done=false;
        bool first = false;
        while(!done){
            if(next == pits_.begin()){
                next = pits_.end();
                next --;
                if(first){
                    error_=Z6IndianErrorEnums::ERROR_SOWING_NOT_POSSIBLE;
                    done = true;
                    stop_ = true;
                }else{
                    first = true;
                }
            }
            if(next->getType(pit_type) && (! compareStr(pit_type, "STORE") ) ){
                bool enable;
                if(next->getEnable(enable) && enable){
                    return next;
                } else{
                    next --;
                }

            } else{
                next --;
            }
        }
    return pits_.end();
}

std::vector<Z6PitJson>::iterator Z6IndianRuleSow::step(std::vector<Z6PitJson>::iterator now){
    int value,prev;
    hand_->getValue(value);
    if(stop_) return now;
    if(value > 0){
        now->getValue(prev);
        prev ++;
        now->setValue(prev);
        value --;
        hand_->setValue(value);
        update_detail(now);
    }else{
        update_hand(now);
    }
    return now;
}

bool Z6IndianRuleCapture::preset(){
    auto next = pits_.begin();
    capture_flag = false;
    while(next != pits_.end()){
        std::string type;
        next->getType(type);
        if( compareStr(type, "STORE") ){
            int side;
            next->getOwner(side);
            if(side == 0){
                store0 = next;
            } else if(side == 1){
                store1 = next;
            }
        }
    }
    return Z6IndianRuleBase::preset();
}

bool Z6IndianRuleCapture::update_store(int side , std::vector<Z6PitJson>::iterator now){
    if(side >= 2) { return false;}
    std::vector<Z6PitJson>::iterator store;
    if(side == 0){
        store = store0;
    }else{
        store = store1;
    }
    int value, seeds;
    now->getValue(value);
    if(value > 0){
        store->getValue(seeds);
        seeds += value;
        store->setValue(seeds);
        now->setValue(0);
        update_detail(now)
        update_detail(store , rule_);
    }
    return true;
}
std::vector<Z6PitJson>::iterator Z6IndianRuleCapture::step(std::vector<Z6PitJson>::iterator now){
    int seed_in_hand = 0;
    int seed_in_pit = 0;

    if(stop_){ return now;}
    now->getValue(seed_in_pit);
    hand_->getValue(seed_in_hand);
    if( capture_flag) {
        if(seed_in_pit > 0){
            update_store(turn_ , now);
        }
        stop_ = true;
    }
    if ((seed_in_hand == 0) && (seed_in_pit == 0)){
        capture_flag = true;
    }
}
 bool Z6IndianRulePasu::preset(){
     old_= prev_=pits_.end();
     return Z6IndianRuleCapture::preset();
 }

std::vector<Z6PitJson>::iterator Z6IndianRulePasu::step(std::vector<Z6PitJson>::iterator now){
    if(stop_) { return now;}
    if(old_ != pits_.end()){
        int value, side;
        old_->getValue(value);
        if(value == pasu_){
            old_->getSide(side);
            update_store(side , old_);
        }
    }
    old_ = prev_;
    prev_ = now;
    return now;
}
std::vector<Z6PitJson>::iterator Z6IndianRuleCaptureX2::step(std::vector<Z6PitJson>::iterator now){
    if(stop_){ return now;}

    int seed_in_hand = 0;
    int seed_in_pit = 0;

    if(stop_){ return now;}
    now->getValue(seed_in_pit);
    hand_->getValue(seed_in_hand);
    if(old_ != pits_.end() && capture_flag){
        int value, side;
        old_->getValue(value);
        if( (value == pasu_) &&(seed_in_pit > 0)){
            update_store(turn_ , old_);
            update_store(turn_ , now);
            stop_ = true;
        }
    }

    if ((seed_in_hand == 0) && (seed_in_pit == 0)){
        capture_flag = true;
    }
    old_ = prev_;
    prev_ = now;
    return now;
}
std::vector<Z6PitJson>::iterator Z6IndianRuleRoundReset::step(std::vector<Z6PitJson>::iterator now) {
    if(!stop_ || (error_ != Z6IndianErrorEnums::ERROR_NO_ERROR){ return now;}

    bool found= false;
    int side, value, side;
    std::string type;
    for(const auto &pit : pits_){
        pit.getType(type);
        if( compareStr(type, "NORMAL")){
            pit.getValue(value);
            pit.getSide(side);
            if( (side != turn_) && (value>0)){
                found = true;
                break;
            }
        }
    }
    roundup_ = !found;
    if( roundup_) {
        roundResetPits();
    }
    return now;
}
bool Z6IndianRuleRoundReset::isGameEnded(){
    bool ret = false;
    Z6RuleJson rule = rule_;
    rule_.setFreeCapture(true);
    for(const auto it=pits_.begin(); it != pits_.end(); it ++){
        it->getType(type);
        if( compareStr(type, "NORMAL")){
            it->getSide(side);
            update_store(side,it);
        }
    }
    int seeds_in_0;
    int seeds_in_1;
    store0->getValue(seeds_in_0);
    store1->getValue(seeds_in_1);
    if((seeds_in_0 < pasu_) || (seeds_in_1 < pasu_)){
        gameover_ = true;
        rule_.setGameEnded(true);
        auto zero_in = (seeds_in_0 <= pasu_)?seeds_in_0:seeds_in_1;
        update_details(zero_in,rule_);
        return true;
    }
    rule_ = rule;

    refillPits();
    return false;
}

bool Z6IndianRuleRoundReset::refillPits(){
    for(const auto &pit : pits_){
        pit.getType(type);
        if( compareStr(type, "NORMAL")){
            int value=0;
            pit.setEnable(false);
            pit.getSide(side);
            auto store = (side ==0)?store0:store1;
            store->getValue(value);
            if(value >= pasu_){
                pit.setEnable(true);
                pit.setValue(pasu_);
                value -= pasu_;
                store->setValue(value);
            }
        }
    }
    return true;
}

Z6IndianRuleSurya& Z6IndianRuleSurya::getInstance(int level){

}
Z6IndianRulePrithiv& Z6IndianRulePrithiv::getInstance(int level){

}
