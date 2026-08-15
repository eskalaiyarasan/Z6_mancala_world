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
#include <memory>
#include <z6_algo_base.h>
#include <pit_schema.h>
#include <root_schema.h>
#include <z6_indian_rule_base.h>
#include <z6_indian_game_mode_base.h>


class Z6IndianClassic: Z6Algo{
protected:
    Z6IndianRuleBase *base_;
    Z6IndianGameModeBase *mode_;
    bool valid_base_;
     Z6IndianClassic(Z6AlgoEnums algo , int level):Z6Algo(algo,level),valid_base_(false), mode(nullptr){}
    Z6BoardRespJson frame_error_response(Z6BoardJson board ,Z6IndianErrorEnums error);
    bool update_response(Z6BoardJson board ,Z6MoveJson move,std::vector<Z6DetailJson>& detail , Z6BoardRespJson &resp );
public:
    bool move(Z6RootJson &root , const Z6MoveReqJson &req , Z6BoardRespJson &resp ) override;
    Z6RootJson init(const  Z6GameJson &game) override;
};
class Z6IndianClassicSurya: Z6IndianClassic{

public:
    Z6IndianClassicSurya(int level);
};

class Z6IndianClassicPrithiv: Z6IndianClassic{
    public:
        Z6IndianClassicPrithiv(int level);

};
