#include "vectorhelpers.hpp"
#include "io.hpp"

Sequence<Object> range(int b, int a, int step)
{
    return Sequence<Object>(a, b, [step](const int &i){ return i + step; });
}

/*
vector<Object> range(int b, int a, int step)
{
    vector<Object> v;
    v.reserve(b);
    for(int i = a; i < b; i += step)
    {
        v.push_back(Object(i));
    }
    return std::move(v);
}
*/
