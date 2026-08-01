/**
 * @file source/singleton/singleton.h
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
#include <string>
#include <unordered_map>


template <typename TContext>
class Z6Storage {
private:
         std::unordered_map<std::string , TContext> store_;
         std::mutex mtx_;
public:
    virtual ~Z6Storage() = default;
    virtual std::unique_ptr<TContext> find(const std::string& id )=0;
};
