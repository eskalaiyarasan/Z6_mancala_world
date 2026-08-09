/**
 * @file source/utils/z6_storage.h
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
#include <mutex>


template <typename TContext>
class Z6Storage {
private:
         std::unordered_map<std::string , TContext> store_;
         std::mutex mtx_;
public:
    TContext* find(const std::string& id ){
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = store_.find(id);
        if (it != store_.end()) {
            return &(it->second);
        }
        return nullptr;
    }
    bool save(const std::string& key,  TContext data){
        std::lock_guard<std::mutex> lock(mtx_);
        store_[std::string(key)] = std::move(data);
        return true;
    }
    bool erase(const std::string& key){
        std::lock_guard<std::mutex> lock(mtx_);
        size_t num_erased = store_.erase(key);
        if (num_erased > 0) {
            return true;
        } else {
            return false;
        }
    }
};
