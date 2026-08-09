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
#include <z6_indian_classic.h>
#include <unordered_map>
#include <utility>
#include <z6_utils.h>

void Z6IndianClassic::reset(){
    Z6PitJson hand;
    hand_ = hand;
    pits_.clear();
    max_index = now = pits_.end();
    store0 = store1 = now;
}
bool Z6IndianClassic::preset(Z6RootJson &root,int pit_index){
    Z6BoardJson board;
    bool ret=false;
    int index;
    if ( !( root.getBoard(board) && board.getPitsVector(pits_)) ) {
        pits_.clear()
        return false;
    }
    auto choice = pits_.end();
    std::string type;
    for (auto it = pits_.begin(); it != pits_.end(); ++it){
        if( it->getIndex(index) &&( index == pit_index) ){
            choice=it;
            choice->getEnabled(ret);
        }
        if(it->getType(type) && (type == "STORE")){
            int pside;
            it->getOwner(pside);
            if (pside == 0){ store0= it;}
            if ( pside == 1){ store1 = it;}
        }
    }

    if(ret){
        //check
        int turn , n;
        int side;
        if( board.getTurn(turn) && (choice->getSide(side)) && (turn == side)){
            hand_ = *choice;
            hand_.setType("HAND");
            choice->setValue(0);
            board.getNpits(n);
            now = choice ;
        }else{
            ret = false;
        }
    }

    if(!ret){
        reset();
        ret = false;
    }

    return ret;
}
void Z6IndianClassic:: update_detail(bool has_rule , Z6RuleJson &rule){
    Z6DetailJson detail;
    detail.setPit(*now);
    if(has_rule){
      detail.setRule(rule);
    }
    details.push_back(detail);
}
bool Z6IndianClassic::capture( int side, Z6RuleJson &rule){
    std::vector<Z6PitJson>::iterator it = pits_.end();
    if(side == 0){
        it = store0;
    } else if(side == 1){
        it = store1;
    }else {
        return false;
    }
    int prev = 0, value ;
    it->getValue(prev);
    now->getValue(value);
    value += prev;
    it->setValue(value);
    now->setValue(0);
    update_detail(true , rule);
}
bool Z6IndianClassic::inc(){
    int prev = 0, value ;
    hand_.getValue(prev);
    std::string type;
    if(prev <=0){
        return false;
    }
    now->getType(type);
    if(type == "STORE"){
        return false;
    }
    now->getValue(value);
    value ++;
    now->setValue(value);
    prev --;
    now->setValue(prev);
    Z6RuleJson rule;
    update_detail(false , rule);
  return true;
}

Z6RootJson Z6IndianClassic::make(int level){
    static const std::unordered_map<int, std::pair<int,int> > base = {
        {1,{4,5}},
        {2,{6,7}},
        {3,{12,7}},
        {4,{4,5}},
        {5,{6,7}},
        {6,{12,7}},
        {7,{4,5}},
        {8,{6,7}},
        {9,{12,7}}
    }
    auto b =base[level];
    Z6BoardJson board;
    board.setActive(true);
    board.setTurn(0);
    board.setNsides(2);
    board.setNseeds(b.first);
    board.setNpits(b.second);
    board.setNround(0);
    std::string visible;
    if (level < 4){
        visible = "TRANSPARENT";
    }else if(level < 7) {
        visible = "TRANSLUCENT";
    }else {
        visible = "OPAQUE";
    }
    board.setVisibility(visible);
    std::string toss = "toss";
    Z6RootJson root;
    root.setschema_version(Z6NameSpace::schema_version);
    root.setBoard(board);
    root.setGameID(Z6EngineSpace::config.id);
    root.setCreatedAt(getISO8601Time());
    root.setStatus(toss);
    return root;
}

bool Z6IndianClassicSurya::move(Z6RootJson &json , const Z6MoveReqJson &req , Z6BoardRespJson &resp ){
    bool ret = true;
    reset();
    int pit_index = -1 , error_num = -1;
    ret = req.getMove(m);
    if(ret) { ret = m.getIndex(pit_index);}
    if(ret){ ret = preset(json , pit_index); } else { error_num = 1;}
    if(ret) { ret = setResetIndex(); } else{ error_num = 2;}
    bool move_end = false;
    if(ret){
        int value = 0;
        ret = hand_.getValue(value);
        ret = (value > 0);
    } else{
        error_num = 3;
    }
    if(!ret){ return false;}
    bool capture;
    while(!move_end){
        now ++;
        std::string type="NORMAL";
        now.getType(type);
        if (type == "STORE"){
            now = max_index;
        }
        if( ! inc()){

        }
    }





}
Z6RootJson Z6IndianClassicSurya::init(const  Z6GameJson &game){
    int level;
    std::string active = "active";
    game.getLevel(level);
    auto root = make(level);
    root.setGame(game);
    root.setStatus(active);
    return root;
}
std::unique_ptr<Z6IndianClassicSurya> Z6IndianClassicSurya::create(int level){
    return std::make_unique<Z6IndianClassicSurya>(level);
}


bool move(Z6RootJson &json , const Z6MoveReqJson &req , Z6BoardRespJson &resp ){

}
Z6RootJson init(const  Z6GameJson &game){
    int level;
    std::string active = "active";
    game.getLevel(level);
    auto root = make(level);
    root.setGame(game);
    root.setStatus(active);
    return root;
}
std::unique_ptr<Z6IndianClassicPrithiv> create(int level){
    return std::make_unique<Z6IndianClassicSurya>(level);
}
