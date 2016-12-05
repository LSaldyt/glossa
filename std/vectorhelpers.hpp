#pragma once
#include <vector>

std::vector<int> range(int a);

template <typename T>
std::vector<T> operator+(std::vector<T> lhs, const std::vector<T>& rhs)
{
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
}

template <typename T>
std::size_t len(const T& t)
{
    return t.size();
}

