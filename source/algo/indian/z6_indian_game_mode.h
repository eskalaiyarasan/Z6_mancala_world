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
#include <vector>
#include <string>
#include <root_schema.h>
#include <movedetail_schema.h>
#include <z6_indian_rule_base.h>
#include <z6_indian_game_mode.h>

enum class Z6IndianGameModeENUM {
    INDIAN_CLASSIC_MODE,
    INDIAN_KINGS_MODE,
    UNKNOWN
};

class Z6IndianGameModeBase {
    protected:
        Z6IndianGameModeBase();
        bool intialized_;
    public:
        std::vector<Z6PitJson> pits_;
        std::vector<Z6DetailJson> details_;
        int seeds_per_pit_;
        int pits_per_side_ ;
        bool pasu_rule_;
        int pasu_seeds_;
        bool roundup_;
        bool gameover_;
        int turn_;
        bool stop_;
        Z6IndianErrorEnums error_;
    static Z6IndianGameModeBase* factory(int level, Z6IndianGameModeENUM mode);
    virtual Z6RootJson& generate()=0;
    virtual ~Z6IndianGameModeBase();
    bool isInited()const { return intialized_;}
};

class Z6IndianGameModeClassic : Z6IndianGameModeBase{
    private:
        int level_;
    public:
        Z6IndianGameModeClassic(int level);
        Z6RootJson generate(Z6GameJson &game) override;
}
