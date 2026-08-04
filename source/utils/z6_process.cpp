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

#include "z6_process.h"


    // Spawn a process and store its PID without waiting
bool Z6Process::Spawn(const std::string& binary, char* const argv[]) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "[Error] Fork failed: " << strerror(errno) << "\n";
            return false;
        }
        else if (pid == 0) {
            // ==========================================
            // CHILD PROCESS
            // ==========================================
            execvp(binary.c_str(), argv);

            // If execvp returns, it failed
            std::cerr << "[Child Error] Failed to execute '" << binary << "': " << strerror(errno) << "\n";
            _exit(1);
        }

        // ==========================================
        // PARENT PROCESS
        // ==========================================
        std::cout << "[Parent] Spawned background process (PID: " << pid << ")\n";
        background_pids_.push_back(pid);
        return true;
    }

    // Call this at the very end of main() to reap all children
void Z6Process::WaitAll() {
        std::cout << "[Parent] Waiting for all background processes to complete...\n";
        for (pid_t pid : background_pids_) {
            int status = 0;
            pid_t waited = waitpid(pid, &status, 0);
            if (waited > 0) {
                if (WIFEXITED(status)) {
                    std::cout << "[Parent] Process " << pid << " exited with code: " << WEXITSTATUS(status) << "\n";
                } else {
                    std::cout << "[Parent] Process " << pid << " terminated abnormally.\n";
                }
            }
        }
        background_pids_.clear();
    }
