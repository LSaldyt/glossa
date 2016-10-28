#pragma once

#include "../std/std.hpp"
#include "animal.hpp"
;

class Dog : public Animal
{
public:
auto sound() -> decltype("bark"s);
;

};

