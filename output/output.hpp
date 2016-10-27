#pragma once

#include "std/std.hpp"
class Animal
{
auto sound() -> decltype("none"s);
;

};

class Dog : public Animal
{
auto sound() -> decltype("bark"s);
;

};

auto main() -> decltype(0);
;

