#pragma once

#include "../std/std.hpp"
class Animal
{
public:
auto sound() -> decltype("none"s);
auto f(auto x) -> decltype(x * 2);

};

