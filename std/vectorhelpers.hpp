#pragma once
#include <vector>
#include <functional>

using std::vector;

vector<int> range(int a);

template <typename T>
vector<T> operator+(vector<T> lhs, const vector<T>& rhs)
{
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
}

template <typename T>
std::size_t len(const T& t)
{
    return t.size();
}

template <typename T, typename U>
vector<U> listComp(const vector<T>&                  v, 
                   std::function<U   (const T&)> trans, 
                   std::function<bool(const T&)> keep  = [](const T&){ return true; })
{
    vector<U> result;
    result.reserve(v.size());
    for (const auto& elem : v)
    {
        if (keep(elem))
        {
            result.push_back(trans(elem));
        }
    }
    return result;
}

