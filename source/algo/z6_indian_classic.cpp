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
#include "z6_algo_base.h"
#include <iostream>
#include <z6_indian_classic.h>
#include <z6_indian_rule_base.h>
#include <z6_indian_rule.h>
#include <z6_indian_game_mode.h>
#include <movereq_schema.h>
#include <game_schema.h>
#include <stdexcept>

bool Z6IndianClassic::move(Z6RootJson &root , const Z6MoveReqJson &req , Z6BoardRespJson &resp ){
    if(! valid_base_) { return valid_base_;}
    Z6MoveJson  move;
    std::vector<Z6PitJson> pits;
    if(! req.getMove(move)){return false;}
    int pitindex, side;
    move.getIndex(pitindex);
    bool ret = base_.apply( pits,side, pitindex);
}
Z6RootJson Z6IndianClassic::init(const  Z6GameJson &game){
    if(mode_){
        return mode_->generate(game);
    }else{
        throw std::runtime_error("algorithm not properly initialized.");
    }
}

Z6IndianClassicSurya::Z6IndianClassicSurya(int level):Z6IndianClassic(INDIAN_CLASSIC_SURYA, level){
    mode_ = Z6IndianGameModeBase::factory(level,Z6IndianGameModeENUM::INDIAN_CLASSIC_MODE);
    auto  surya = Z6IndianRuleBase::factory(Z6IndianGameType::INDIAN_GAME_SURYA , level);
    if(mode->pasu_rule_){
        //No pasu rule, no double capture
        base_ = new Z6IndianRuleRoundReset(mode->seeds_per_pit_,
            Z6IndianRulePasu(mode_->pasu_seeds_  ,
            Z6IndianRuleSow(
                Z6IndianRuleCapture(
                    Z6IndianRuleCaptureX2(mode_->pasu_seeds_ ,*surya)))));

    }else{
        base_ = new Z6IndianRuleRoundReset(mode->seeds_per_pit_,
            Z6IndianRuleSow(
                Z6IndianRuleCapture(*surya)));
    }
}

Z6IndianClassicPrithiv::Z6IndianClassicPrithiv(int level):Z6IndianClassic(Z6AlgoEnums::INDIAN_CLASSIC_PRITHIV, level){
    mode_ = Z6IndianGameModeBase::factory(level,Z6IndianGameModeENUM::INDIAN_CLASSIC_MODE);
    auto  prithiv = Z6IndianRuleBase::factory(Z6IndianGameType::INDIAN_GAME_PRITHIV , level, mode_);
    if(mode->pasu_rule_){
        //No pasu rule, no double capture
        base_ = new Z6IndianRuleRoundReset(mode->seeds_per_pit_,
            Z6IndianRulePasu(mode_->pasu_seeds_  ,
            Z6IndianRuleSow(
                Z6IndianRuleCapture(
                    Z6IndianRuleCaptureX2(mode_->pasu_seeds_ ,*prithiv)))));

    }else{
        base_ = new Z6IndianRuleRoundReset(mode->seeds_per_pit_,
            Z6IndianRuleSow(
                Z6IndianRuleCapture(*prithiv)));
    }
}
