/**
 * @file source/Z6Singleton/Z6Singleton.h
 * @author  kalaiyarasan S
 * @date 2026-08-01
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */
#include <z6_singleton.h>
#include <fstream>
#include <string>
#define MAP_NPLAYERS_NAME   "/schema/z6_players_required.json"
Z6ChartNplayers::Z6ChartNplayers()
{
    std::ifstream file("data.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open data.json\n";
        return 1;
    }
    try {
            // 2. Parse the JSON file
            json j;
            file >> j;

            // 3. Handle the array format: [ { "apple": 10 }, { "banana": 20 }, ... ]
            if (j.is_array()) {
                for (const auto& item : j) {
                    // Each item is an object containing a key-value pair
                    for (auto it = item.begin(); it != item.end(); ++it) {
                        std::string key = it.key();
                        int value = it.value().get<int>();
                        _store.save(key, value);
                    }
                }
            } else {
                std::cerr << "Error: Root JSON is not an array.\n";
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << '\n';
        }
}
