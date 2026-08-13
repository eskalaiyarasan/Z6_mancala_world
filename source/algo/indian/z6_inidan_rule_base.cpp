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
#include <iostream>
#include <z6_indian_rule_base.h>

Z6IndianRuleBase::Z6IndianRuleBase(const Z6IndianRuleBase &other):
    hand_(other.hand_), pits_(other.pits_),
    child_(other),roundup_(other_.roun),
    details_(other.details_),gameover_(other.gameover_),
    turn_(other.turn_),stop_(other.stop_),error_(other.error_):valid_child_(true) {}

bool Z6IndianRuleBase::initial_check(std::vector<Z6PitJson>::iterator now){
    int side;
    std::sting pit_type;
    error_= Z6IndianErrorEnums::ERROR_NO_ERROR ;
    if(pits_ = nullptr) {return Z6IndianErrorEnums::ERROR_PIT_NOT_EXIST;}

    if(now >= pits_->end() || now< pits_->begin()){
        error_ = Z6IndianErrorEnums::ERROR_PIT_NOT_EXIST;
    }
    if((turn >=2) || (now->getSide(side) == false){
        error_ = Z6IndianErrorEnums::ERROR_BEYOND_BOUNDARY;
    } else{
        error_=(turn == side)?Z6IndianErrorEnums::ERROR_NO_ERROR : Z6IndianErrorEnums::ERROR_NOT_YOUR_TURN;
    }
    if ( !(now->getType(pit_type) && compareStr(pit_type, "NORMAL")) ){
        error = Z6IndianErrorEnums::ERROR_CHOSEN_INVALID_PIT;
    }
    error_ = update_hand(now)?Z6IndianErrorEnums::ERROR_NO_ERROR : Z6IndianErrorEnums::ERROR_CHOSEN_INVALID_PIT;

    return (error_ == Z6IndianErrorEnums::ERROR_NO_ERROR);
}
bool Z6IndianRuleBase::apply(std::vector<Z6PitJson> pits, int side , int pitindex){
    bool ret = false;
    //find corresponding iterator
    std::vector<Z6PitJson>::iterator now = pits.begin();
    int index = -1;
    while(now != pits.end()){
        if( now->getIndex(index) && (index == pitindex) ){
            break;
        }
        now ++;
    }
    stop_ = false;
    details_.clear();
    roundup_ = gameover_ = false;
    turn_= side;
    update_pits_reference(pits);
    if(! initial_check(now)) { return false;}
    if( !preset()) { return false;}
    while(!stop_){
        if(valid_child_){
            now = child_.step(now);
        }

        step(now)
    }
    return true;
}

bool Z6IndianRuleBase::preset(){

    if(valid_child_){
        return child_.preset();
    }
    return true;
}
bool Z6IndianRuleBase::update_hand(std::vector<Z6PitJson>::iterator now){
    int value;
    now->getValue(value);
    now->setValue(0);
    hand_->setValue(value);
    Z6RuleJson rule;
    update_detail(now);
}
void Z6IndianRuleBase:: update_detail(std::vector<Z6PitJson>::iterator now){
    Z6DetailJson detail;
    detail.setPit(*now);
    details.push_back(detail);
}
void Z6IndianRuleBase:: update_detail(std::vector<Z6PitJson>::iterator now, Z6RuleJson &rule){
    Z6DetailJson detail;
    detail.setPit(*now);
    detail.setRule(rule);
    details.push_back(detail);
}

bool Z6IndianRuleBase::update_pits_reference(std::vector<Z6PitJson> &pits){
    if(valid_child_){
        child_.update_pits_reference(pits);
    }
    pits_ = &pits;
}
