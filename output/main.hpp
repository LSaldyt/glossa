#pragma once

#include "../std/std.hpp"
class Animal
{
public:
std::tuple<> __members;
std::unordered_map<std::string, int> __access_map;
Animal();
auto sound() -> decltype("none"s);

};

class Dog : public Animal
{
public:
std::tuple<> __members;
std::unordered_map<std::string, int> __access_map;
Dog();
auto sound() -> decltype("bark"s);

};

auto main() -> decltype(0);

