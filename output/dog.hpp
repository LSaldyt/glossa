#pragma once

#include "animal.hpp"
;

class Dog : public Animal
{
auto sound() -> decltype("bark"s);
;

};

