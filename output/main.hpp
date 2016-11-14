#pragma once

#include "../std/std.hpp"
template < typename T_name, typename T_sound >
class __Animal__
{
public:
T_name name;
T_sound sound;
__Animal__(T_name set_name, T_sound set_sound) : name(set_name), sound(set_sound) {}
void make_sound()
{
print(name + " makes: "s + sound);

};

};
const auto Animal = [=](auto set_name, auto set_sound){
print("Creating Animal"s);
auto name = set_name;
auto sound = set_sound;
return __Animal__<decltype(name), decltype(sound)>(name, sound);
};
;

template < typename T_inheritance >
class __Dog__ : public T_inheritance
{
public:
__Dog__(auto set_inheritance) : T_inheritance(set_inheritance) {}

};
const auto Dog = [=](auto set_name){
print("Creating dog "s + set_name);
auto set_inheritance = Animal(set_name, "bark"s);
return __Dog__<decltype(set_inheritance)>(set_inheritance);
};
;

template < typename T_inheritance >
class __GoldenRetriever__ : public T_inheritance
{
public:
__GoldenRetriever__(auto set_inheritance) : T_inheritance(set_inheritance) {}

};
const auto GoldenRetriever = [=](auto set_name){
print("Creating golden retriever "s + set_name);
auto set_inheritance = Dog(set_name);
return __GoldenRetriever__<decltype(set_inheritance)>(set_inheritance);
};
;

auto main() -> decltype(0);

