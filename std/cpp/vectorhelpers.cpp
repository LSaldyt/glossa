#include "vectorhelpers.hpp"
#include "io.hpp"

vector<Object> range(int b, int a, int step)
{
    vector<Object> v;
    v.reserve(b);
    for(int i = a; i < b; i += step)
    {
        v.push_back(Object(i));
    }
    return v;
}
