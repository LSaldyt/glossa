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

template < typename t_type, typename t_inheritance >
class __dog__ : public t_inheritance
{
public:
t_type type;
__dog__(t_type set_type, t_inheritance set_inheritance) : type(set_type), t_inheritance(set_inheritance) {}

};
template < typename t_set_name, typename t_set_type>
auto dog(t_set_name set_name, t_set_type set_type)
{
    return __dog__<decltype(set_type), decltype(animal(set_name, "bark"s))>(set_type, animal(set_name, "bark"s));
};

template < typename T_x >
 auto foo(T_x x)
{
    if (x){ return "Yes"s;
} else { return "No"s;
};

};

// No main declaration required;

