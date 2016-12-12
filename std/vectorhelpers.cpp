#include "vectorhelpers.hpp"

std::vector<int> range(int b, int a, int step)
{
    std::vector<int> indices;
    indices.reserve(b);
    for (int i = a; i < b; i += step)
    {
        indices.push_back(i);
    }
    return indices;
}
