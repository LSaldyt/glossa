#include "std/std.hpp"

class Animal
{
auto sound() -> decltype("none"s)
{
return "none"s;
};

};

auto main() -> decltype(0)
{
print("Hello World"s);
return 0;
};

