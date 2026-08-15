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
#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>
#include <z6_utils.h>
#include <pit_schema.h>
#include <root_schema.h>
#include <z6_indian_game_mode.h>

enum class Z6IndianErrorEnums {
    ERROR_NO_ERROR = 0,
    ERROR_NOT_YOUR_TURN    = 2,
    ERROR_BEYOND_BOUNDARY  = 3,
    ERROR_CHOSEN_INVALID_PIT = 4,
    ERROR_PIT_NOT_EXIST = 5,
    ERROR_SOWING_NOT_POSSIBLE = 6,
    ERROR_UNKNOWN_ERROR,
};
enum class Z6IndianGameType {
    INDIAN_GAME_SURYA = 1,
    INDIAN_GAME_PRITHIV = 2;
};
class Z6IndianRuleBase{
    private:
        bool valid_child_;
    protected:
        Z6PitJson *hand_;
        std::vector<Z6PitJson> *pits_;
        const Z6IndianRuleBase &child_;
        std::vector<Z6DetailJson> &details_;
        bool &roundup_;
        bool &gameover_;
        int &turn_;
        bool &stop_;
        Z6IndianErrorEnums &error_;
        virtual bool initial_check(std::vector<Z6PitJson>::iterator now);
        bool preset();
        bool update_hand(std::vector<Z6PitJson>::iterator);
        void update_detail(std::vector<Z6PitJson>::iterator now);
        void update_detail(std::vector<Z6PitJson>::iterator now, Z6RuleJson &rule);
        bool update_pits_reference(std::vector<Z6PitJson> &pits);
        Z6IndianRuleBase(Z6IndianGameModeBase *p);
    public:
        Z6IndianRuleBase(const Z6IndianRuleBase& other);
        virtual std::vector<Z6PitJson>::iterator step(std::vector<Z6PitJson>::iterator now) = 0;
        bool apply(std::vector<Z6PitJson> &pits,int side, int pitindex);
        virtual ~Z6IndianRuleBase(){}
        static Z6IndianRuleBase* factory( Z6IndianGameModeBase *m){ return nullptr}
        bool get_roundup()const { return roundup_;}
        Z6IndianErrorEnums get_error() const { return error_;}
        bool isGameover()const { return gameover_;}
};
