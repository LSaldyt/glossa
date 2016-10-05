#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "IO.hpp"
template<typename T>
void print(T t) {
        std::cout << t << "\n";
}

template<typename T, typename... Args>
void print(T first, Args... args) {
    print(first);
    print(args...);
    std::cout << "\n";
}

namespace tools
{


}
