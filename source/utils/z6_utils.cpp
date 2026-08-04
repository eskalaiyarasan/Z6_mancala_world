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
 #include "z6_uuid.h"

 std::string  z6_uuid(){
    uuid_t binuuid;
    uuid_generate_random(binuuid);

     char uuid_str[37];
     uuid_unparse(binuuid, uuid_str);

     return uuid_str;
 }
