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
const auto Animal = [=](){
print("Creating Animal"s);
auto name = "none"s;
auto sound = "none"s;
return __Animal__<decltype(name), decltype(sound)>(name, sound);
};
;

template < typename T_name, typename T_sound, class T_inheritance >
class __Dog__ : public T_inheritance
{
public:
T_name name;
T_sound sound;
__Dog__(T_name set_name, T_sound set_sound) : name(set_name), sound(set_sound), T_inheritance(Animal()) {}

};
const auto Dog = [=](auto set_name){
print("Creating dog "s + set_name);
auto name = set_name;
auto sound = "bark"s;
return __Dog__<decltype(name), decltype(sound), decltype(Animal())>(name, sound);
};
;

auto main() -> decltype(0);

