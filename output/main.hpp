#pragma once

#include "../std/std.hpp"
template < typename T_name, typename T_sound >
class __Animal__
{
public:
T_name name;
T_sound sound;
__Animal__(T_name set_name, T_sound set_sound) : name(set_name), sound(set_sound) {}
     auto make_sound()
{
    print(name + " makes "s + sound);

};

};
template < typename T_set_name, typename T_set_sound>
auto Animal(T_set_name set_name, T_set_sound set_sound)
{
    return __Animal__<decltype(set_name), decltype(set_sound)>(set_name, set_sound);
};

template < typename T_inheritance >
class __Dog__ : public T_inheritance
{
public:
__Dog__(T_inheritance set_inheritance) : T_inheritance(set_inheritance) {}

};
template < typename T_set_name>
auto Dog(T_set_name set_name)
{
    return __Dog__<decltype(Animal(set_name, "bark"s))>(Animal(set_name, "bark"s));
};

template < typename T_x >
 auto foo(T_x x)
{
    if (x){ return "Yes"s;
} else { return "No"s;
};

};

// No main declaration required;

