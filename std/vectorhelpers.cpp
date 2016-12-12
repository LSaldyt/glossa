#include "vectorhelpers.hpp"

vector<Object> range(Object b, Object a, Object step)
{
    vector<Object> indices;
    //indices.reserve(b);
    for (Object i = a; i < b; i += step)
    {
        indices.push_back(i);
    }
    return indices;
}
