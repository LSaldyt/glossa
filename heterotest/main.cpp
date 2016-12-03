#include "object.hpp"

#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<Object> objects;
    objects.push_back(1);
    objects.push_back(true);

    for (auto o : objects)
    {
        std::cout << str(o) << std::endl;
    }
}
