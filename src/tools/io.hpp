#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

#include "base.hpp"

namespace tools
{

/// Abstract IO
std::vector<std::string> readFile(string filename);
/// Abstract IO
void writeFile(vector<string> content, string filename);

/// Abstract IO
template <typename T>
void print(T t) {
    std::cout << t << "\n";
}

/// Abstract IO
template <typename T, typename... Args>
void print(T first, Args... args) {
    print(first);
    print(args...);
    std::cout << "\n";
}
}
