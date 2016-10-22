#include <vector>

std::vector<int> range(int a)
{
    std::vector<int> indices;
    indices.reserve(a);
    for (int i = 0; i < a; i ++)
    {
        indices.push_back(i);
    }
    return indices;
}
