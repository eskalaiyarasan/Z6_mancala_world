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


class Z6IndianClassic : Z6Algo{
    public:
        Z6IndianClassic(Z6AlgoEnums algo , int level):Z6Algo(algo ,level){}
    protected:
        static const int MAX_SIDE =2;
        std::vector<Z6PitJson>::iterator  store0;
        std::vector<Z6PitJson>::iterator  store1;
        Z6PitJson hand_;
        std::vector<Z6PitJson> pits_;
        std::vector<Z6PitJson>::iterator now;
        std::vector<Z6PitJson>::iterator old;
        std::vector<Z6DetailJson> details;
        std::vector<Z6PitJson>::iterator max_index;
        void reset();
        bool preset(Z6RootJson &root,int pit_index);
        bool capture(int side, Z6RuleJson &rule);
        bool inc();
        void update_detail(bool norule , Z6RuleJson &rule);
        Z6RootJson make(int level);

};

class Z6IndianClassicSurya: Z6IndianClassic{
private:
        Z6IndianClassicSurya(int level):Z6IndianClassic(INDIAN_CLASSIC_SURYA,level){}
        void setResetIndex();
public:
    bool move(Z6RootJson &json , const Z6MoveReqJson &req , Z6BoardRespJson &resp ) override;
    Z6RootJson init(const  Z6GameJson &game) override;
    static std::unique_ptr<Z6IndianClassicSurya> create(int level);
};

class Z6IndianClassicPrithiv: Z6IndianClassic{
    private:
        Z6IndianClassicPrithiv(int level):Z6IndianClassic(INDIAN_CLASSIC_PRITHIV,level){}
        void setResetIndex();
    public:
        bool move(Z6RootJson &json , const Z6MoveReqJson &req , Z6BoardRespJson &resp ) override;
        Z6RootJson init(const  Z6GameJson &game)  override;
        static std::unique_ptr<Z6IndianClassicPrithiv> create(int level);
};
