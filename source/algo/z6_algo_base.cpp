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

#include <z6_algo_base.h>
#include <z6_algo_factory.h>
#include <z6_indian_classic.h>



std::unique_ptr<Z6Algo> Z6Algo::produce(const  Z6GameJson &game){
    std::string name;
    int level;
    game.getName(name);
    game.getLevel(level);

    if(compareStr(name, "INDIAN_CLASSIC_SURYA")){
        return std::make_unique<Z6IndianClassicSurya>(level);
    }
    if(compareStr(name , "INMDIAN_CLASSIC_PRITHIV")){
        return std::make_unique<Z6IndianClassicPrithiv>(level);
    }
    return nullptr;
}

bool jsonFactory_produce(const Z6GameJson &game , Z6RootJson &root, std::unique_ptr<Z6Algo>  &algo){
    bool ret=false;
    auto it = Z6Algo::produce(game);
    if (it && it.is_unknown){ return false;}
    root = it->init(game);
    algo = std::move(it);
    return true;
}
