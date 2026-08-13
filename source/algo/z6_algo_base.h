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
#include <iostream>
#include <memory>
#include <z6_base_json.h>
#include <movereq_schema.h>
#include <moveresp_schema.h>
#include <game_schema.h>
#include <stdexcept>
#include <root_schema.h>
#include <unordered_map>
#include <functional>
#include <any>
#include <string>

enum class Z6AlgoEnums {
    INDIAN_CLASSIC_SURYA,
    INDIAN_CLASSIC_PRITHIV,
    INDIAN_KINGS_SURYA,
    INDIAN_KINGS_PRITHIV,
    UNKNOWN
};
namespace Z6EngineSpace {
    public:
    using GenericFunc = std::function<std::any()>;
    extern const std::unordered_map<std::string, GenericFunc> registry;
}
class Z6Algo{
    protected:
    int level_;
    Z6AlgoEnums algo_;
    public:
        Z6Algo():algo_(Z6AlgoEnums::UNKNOWN) , level_(-1){}
        Z6Algo(Z6AlgoEnums algo , int level):level_(level), algo_(algo){}
        ~Z6Algo();
        bool is_unknown() const { return (level_ == Z6AlgoEnums::UNKNOWN)}
        virtual bool move(Z6RootJson &root , const Z6MoveReqJson &req , Z6BoardRespJson &resp )=0;
        static std::unique_ptr<Z6Algo> produce(Z6GameJson &game);
        virtual Z6RootJson init(const  Z6GameJson &game) = 0;
};


bool jsonFactory_produce(const Z6GameJson &game , Z6RootJson &root , std::unique_ptr<Z6Algo>  &algo);
