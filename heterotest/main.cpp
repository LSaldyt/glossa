#include "object.hpp"

#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<Object> objects;
    objects.push_back(1);
    objects.push_back(true);

    const auto print = [](auto v)
    {
        std::cout << v << std::endl;
    };

    for (auto o : objects)
    {
        print(o);
    }

    Object a = 1;
    Object b = 2;

    print(a < b);
    print(a < 2);
    print(a > 2);
}
