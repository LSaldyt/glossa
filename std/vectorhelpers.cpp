#include "vectorhelpers.hpp"
#include "io.hpp"

vector<int> range(int b, int a, int step)
{
    vector<int> v;
    v.reserve(b);
    for(int i = a; i < b; i += step)
    {
        v.push_back(i);
    }
    return v;
}
