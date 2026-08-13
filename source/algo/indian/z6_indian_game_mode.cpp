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
#include <z6_indian_game_mode.h>
#include <z6_utils.h>
#include <z6_base_json.h>
$include <z6_engine_singleton.h>
static const int _seeds_per_pit_array[] ={
    4,6,12,
    4,6,12,
    4,6,12,12
};
static const int _pits_per_side_array[] ={
    5,7,7,
    5,7,7,
    5,7,7,7
};
static const int _pasu_seeds_array[] = {
  0,4,6,
  0,4,6,
  0,4,6,6
};
static const std::string _transparency[] ={"TRANSPARENT","TRANSLUCENT","OPAQUE"};

static Z6PitJson _make_pit(int side, int index, std::string &type, int seeds=0){
    Z6PitJson pit;
    pit.setEnabled(true);
    pit.setType(type);
    pit.setIndex(index);
    pit.setSide(side);
    if(compareStr(type , "STORE")){
        pit.setOwner(side);
    }
    pit.setValue(seeds);
    return pit;
}

Z6IndianGameModeBase::Z6IndianGameModeBase():intialized_(false){

}
Z6IndianGameModeBase* factory(int level, Z6IndianGameModeENUM mode){

    if(mode == INDIAN_CLASSIC_MODE){
        auto classic = new Z6IndianGameModeClassic(level);
        return classic;
    }

}
Z6IndianGameModeClassic::Z6IndianGameModeClassic(int level):Z6IndianGameModeBase(), level_(level){
    seeds_per_pit_=_seeds_per_pit_array[level];
    pits_per_side_ = _pits_per_side_array[level];
    pasu_rule_ = (_pasu_seeds_array[level > 0]);
    pasu_seeds_ = _pasu_seeds_array[level] ;
    roundup_ = false;
    gameover_ = false;
    turn_=0;
    stop_ = true;
    error_ = Z6IndianErrorEnums::ERROR_NO_ERROR;
    int index = 0, side = 0;

    pits_.push_back(_make_pit_store(side,index,"STORE", 0));
    int max_pits = 2*pits_per_side_;
    for(int i = 0; i<max_pits; i++){
        index ++;
        if(i == pits_per_side_){
            side ++;
        }
        pits_.push_back(_make_pit_store(side,index,"NORMAL",seeds_per_pit_));
    }
    index++;
    pits_.push_back(_make_pit_store(side,index,"STORE", 0));
}
Z6RootJson Z6IndianGameModeClassic::generate(Z6GameJson &game){
    Z6RootJson root;
    Z6BoardJson board;
    root.setschema_version(Z6_SCHEMA_VERSION);
    root.setGameID(Z6EngineSpace::config.id);
    root.setGame(game);
    root.setCreatedAt(getISO8601Time());
    root.setStatus("active");
    board.setActive(true);
    board.setTurn(turn_);
    board.setNsides(2);
    board.setNseeds(seeds_per_pit_);
    board.setNpits(pits_per_side_);
    board.setPits(pits_);
    board.setNround(1);
    auto visible = _transparency [ (level_-1)/3];
    board.setVisibility(visible);
    root.setBoard(board);
    return root;
}
