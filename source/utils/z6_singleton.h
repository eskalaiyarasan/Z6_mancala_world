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
#pragma once
#include <iostream>
#include <mutex>


// Base class template enforcing Z6Singleton behavior on derived classes
template <typename Derived>
class Z6Singleton {
public:
    // Delete copy and move constructors/assignments
    Z6Singleton(const Z6Singleton&) = delete;
    Z6Singleton& operator=(const Z6Singleton&) = delete;
    Z6Singleton(Z6Singleton&&) = delete;
    Z6Singleton& operator=(Z6Singleton&&) = delete;

    // Global access point for the derived Z6Singleton instance
    static Derived& getInstance() {
        static Derived instance; // Thread-safe in C++11 and later
        return instance;
    }

protected:
    // Protected constructor so derived classes can initialize themselves,
    // but external code cannot instantiate them directly using 'new' or stack allocation.
    Z6Singleton() = default;
    virtual ~Z6Singleton() = default;
};
