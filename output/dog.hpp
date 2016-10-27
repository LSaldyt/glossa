#pragma once

#include "animal.hpp"
;

class Dog : public Animal
{
public:
auto sound() -> decltype("bark"s);
;

};

