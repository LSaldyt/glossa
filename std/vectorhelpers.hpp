#pragma once
#include <vector>
template <typename T>
void append(std::vector<T>& v, const T& t)
{
    v.push_back(t);
}

std::vector<int> range(int a);
