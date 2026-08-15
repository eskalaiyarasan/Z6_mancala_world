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
#include "indian/z6_indian_rule_base.h"
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
    Z6BoardJson board;
    root.getBoard(board);
    board.getTurn(side);
    board.getPitsVector(pits);
    bool ret = base_->apply( pits,side, pitindex);
    if(!ret ){
        resp = frame_error_response(base_->get_error());
        return false;
    }
    side ++;
    side = side % 2;
    board.setTurn(side);
    if(base_->get_roundup()){
        int round;
        board.getRound(round);
        round ++;
        board.setRound(round);
    }
    board.setPits(*(base_->get_pits_ptr()) );
    Z6MoveHistoryJson history;
    std::vector<Z6MoveHistoryJson> history_vector;
    history.setMove(move);
    history.setDetails(base_->get_detail_ref());
    board.getHistoryVector(history_vector);
    history_vector.push_pack(history);
    history.setHistoryVector(history_vector);
    update_response(board,move ,base_->get_detail_ref(),resp);
    root.setBoard(board);
    if (base_->isGameover()){
        //update gameover here.
        Z6ResultJson result;
        result.setReport("Result");
        resp.setResult(result);
        root.setResult(result);
    }
    return true;
}
Z6BoardRespJson Z6IndianClassic::frame_error_response(Z6BoardJson &board, Z6MoveJson  move,Z6IndianErrorEnums error){
    Z6BoardRespJson resp;
    resp.setschema_version(Z6_SCHEMA_VERSION);
    bool active=false;
    board.getActive(active);
    resp.setActive(active);
    resp.setGameID(Z6EngineSpace::config.id);
    int turn;
    board.getTurn(turn);
    resp.setturn(turn);
    int round;
    board.getRound(round);
    resp.getRound(round);
    int sequence;
    move.getSequence(sequence);
    resp.setSequence(sequence);
    std::vector<Z6MoveJson> moves;
    std::vector<Z6MoveHistoryJson> history_vector;
    board.getHistoryVector(history_vector);
    for(conat auto history : history_vector){
        history.getMove(move);
        moves.push_back(move);
    }
    resp.setHistory(moves);
    if(error != Z6IndianErrorEnums::ERROR_NO_ERROR){
        resp.setType("ERROR");
        Z6ErrorJson jerror;
        switch(error){
            case Z6IndianErrorEnums::ERROR_BEYOND_BOUNDARY:
                jerror.setBeyond_boundary(true); break;
            case Z6IndianErrorEnums::ERROR_CHOSEN_INVALID_PIT:
                jerror.setChosen_invalid_pit(true); break;
            case Z6IndianErrorEnums::ERROR_NOT_YOUR_TURN:
                jerror.setNot_your_turn(true); break;
            case Z6IndianErrorEnums::ERROR_PIT_NOT_EXIST:
                jerror.setPit_not_exist(true);break;
            case Z6IndianErrorEnums::ERROR_SOWING_NOT_POSSIBLE:
                jerror.setSowing_not_possible(true); break;
            default:
                jerror.setUnknown_condition(true);
        }
        resp.setError(jerror);
    }
    return resp;
}
bool Z6IndianClassic::update_response(Z6BoardJson &board , Z6MoveJson  move,std::vector<Z6DetailJson> &details, Z6BoardRespJson &resp){
    resp = frame_error_response(board , move, Z6IndianErrorEnums::ERROR_NO_ERROR);
    resp.setType("OK");
    resp.setDetails(details);
    return true;
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
    auto  surya = Z6IndianRuleBase::factory(Z6IndianGameType::INDIAN_GAME_SURYA , mode_);
    if(mode_->pasu_rule_){
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
