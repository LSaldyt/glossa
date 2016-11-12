#pragma once

#include "../std/std.hpp"
class Animal
{
public:
Animal();auto sound() -> decltype("none"s);

};

class Dog : public Animal
{
public:
Dog();auto sound() -> decltype("bark"s);

};

auto main() -> decltype(0);

