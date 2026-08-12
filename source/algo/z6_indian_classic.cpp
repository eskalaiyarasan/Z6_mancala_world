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
class Z6IndianClassic_priv{
    inline static const std::string visibility[]={
        "TRANSPARENT","TRANSLUCENT","OPAQUE"
    };
  public:
      int nSeeds;
      int nPits;
      int visible;
      int pasu;
      Z6RulesJson rule_;
      inline const std::string& getVisible(){ return visibility[visible];}
      Z6IndianClassic_priv(int s, int p , int i , int r , int a):
        nSeeds(s), nPits(p),visible(i),pasu(a){

            rule_.setSowing(true);
            rule_.setCapture(true);
            rule_.setRoundReset(true);
            rule_.setFreeCapture(true);
            if(r > 0){
                rule_.setPasu(true);
                rule_.setDoubleCapture(true);
            }
            if(visible > 2){
                visible = 2;
            }
        }

};

static const  Z6IndianClassic_priv base[] = {

    Z6IndianClassic_priv(4,5,0,0,0),
    Z6IndianClassic_priv(6,7,0,1,4),
    Z6IndianClassic_priv(12,7,0, 1,6),
    Z6IndianClassic_priv(4,5,1,0,0),
    Z6IndianClassic_priv(6,7,1, 1,4),
    Z6IndianClassic_priv(12,7,1,1,6),
    Z6IndianClassic_priv(4,5,2,0,0),
    Z6IndianClassic_priv(6,7,2, 1,4),
    Z6IndianClassic_priv(12,7,2,1,6)
}
void Z6IndianClassic::reset(){
    Z6PitJson hand;
    hand_ = hand;
    pits_.clear();
    max_index = now = pits_.end();
    store0 = store1 = now;
    details.clear();
}
void Z6IndianClassic::update_hand(){
    int value;
    now->getValue(value);
    now->setValue(0);
    hand_.setValue(value);
    Z6RuleJson rule;
    update_detail(false , rule);

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
        int turn ;
        int side;
        if( board.getTurn(turn) && (choice->getSide(side)) && (turn == side)){
            hand_ = *choice;
            hand_.setType("HAND");
            now = choice ;
            update_hand()
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


    Z6BoardJson board;
    board.setActive(true);
    board.setTurn(0);
    board.setNsides(2);
    board.setNseeds(base[level].nSeeds);
    board.setNpits(base[level].nPits);
    board.setNround(0);
    board.setVisibility(base[level].getVisible());
    std::string toss = "toss";
    Z6RootJson root;
    root.setschema_version(Z6NameSpace::schema_version);
    root.setBoard(board);
    root.setGameID(Z6EngineSpace::config.id);
    root.setCreatedAt(getISO8601Time());
    root.setStatus(toss);
    return root;
}
void Z6IndianClassic::update_root(Z6RootJson &root , Z6MoveJson &m ){

    std::vector<Z6MoveHistoryJson> val;
    req.getMove(m);
    Z6MoveHistoryJson history;
    history.setMove(m);
    history.setDetails(details);
    board.getHistoryVector(val);
    val.push_back(history);
    board.setHistory(val);
    board.setPits(pits_);
    root.setBoard(board);
}
bool Z6IndianClassicSurya::move(Z6RootJson &root , const Z6MoveReqJson &req , Z6BoardRespJson &resp ){
    bool ret = true;
    reset();
    int pit_index = -1 , error_num = -1;
    Z6MoveJson m;
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

    while(!move_end){
        old = now;
        now ++;
        std::string type="NORMAL";
        now.getType(type);
        if (type == "STORE"){
            now = max_index;
        }
         if( ! inc()){
            int value;
            update_hand();
            hand_.getValue(value);
            if(value == 0){ move_end = true;}
        }
    }

    now++;
    int side;
    board.getTurn(side)
    Z6RuleJson rule;
    rule.setCapture(true);
    capture(side , rule);
    update_root(root,m)
    frame_resp()

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
