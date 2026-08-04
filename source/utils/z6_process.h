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
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

class Z6Process {
private:
    std::vector<pid_t> background_pids_;

public:
    // Spawn a process and store its PID without waiting
    bool Spawn(const std::string& binary, char* const argv[]) ;

    // Call this at the very end of main() to reap all children
    void WaitAll();

    // Destructor automatically cleans up if WaitAll wasn't called manually
    ~Z6Process() {
        if (!background_pids_.empty()) {
            WaitAll();
        }
    }
};
