#include "output.hpp"
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
if (true){ print("Hello World"s);
} else { };
return 0;
};

