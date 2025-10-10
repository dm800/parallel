#pragma once
#include <chrono>
#include <cassert>


template<typename ... Args>
long long TimeCheck(bool func(Args...), Args ... args) {
    auto start = std::chrono::steady_clock::now();
    if (!func(args...)) {
        return -1;
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    return duration;
}

