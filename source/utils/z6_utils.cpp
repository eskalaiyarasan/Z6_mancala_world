/**
 * @file source/utils/z6_utils.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
 #include "z6_utils.h"
#include <chrono>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

 namespace Z6NameSpace {
     int nSocketClient = 2;
     int schema_version = 1;
 };
 std::string  z6_uuid(){
    uuid_t binuuid;
    uuid_generate_random(binuuid);

     char uuid_str[37];
     uuid_unparse(binuuid, uuid_str);

     return uuid_str;
 }
std::string getISO8601Time(){

    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm utc_tm;

    gmtime_r(&time_now , &utc_tm);
    std::stringstream ss;
    ss<<std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}
// Trim function accepting a const std::string& (or implicitly convertible string_view)
std::string_view trim(const std::string& str) {
    std::string_view sv(str);

    // Trim leading spaces
    auto start = std::find_if_not(sv.begin(), sv.end(), [](unsigned char c) {
        return std::isspace(c);
    });

    // Trim trailing spaces
    auto end = std::find_if_not(sv.rbegin(), sv.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();

    return (start < end) ? std::string_view(start, std::distance(start, end)) : std::string_view{};
}
bool compareStr(const std::string& s1, const std::string& s2)
{
    return trim(s1) == trim(s2);
}
