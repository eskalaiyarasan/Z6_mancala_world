/**
 * @file source/utils/z6_uuid.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */

#pragma once
#include <uuid/uuid.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string_view>



namespace Z6NameSpace {
    extern int nSocketClient;
    extern int schema_version;
};

std::string  z6_uuid();
std::string getISO8601Time();
std::string_view trim(const std::string& str);
bool compareStr(const std::string& s1, const std::string& s2);
