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
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Z6Json {
public:
    virtual ~Z6json() = default;
    virtual void from_json(const std::string& json_str) = 0;
    virtual std::string to_json() const = 0;
};
class Z6BoardJson;
class Z6GameJson;
class Z6MoveHistoryJson;
class Z6MoveJson;
class Z6DetailJson;
class Z6MoveReqJson;
class Z6BoardRespJson;
class Z6PitJson;
class Z6RootJson;
class Z6RulesJson;
