#pragma once

#include "../std/std.hpp"
template < typename T_name > class __Animal__
{
public:
T_name name;
/*no header*/;
/*no header*/;
__Animal__(T_name set_name) : name(set_name) {}
auto sound() -> decltype("none"s)
{
return "none"s;
};

};
const auto Animal = [=](){
print("Creating Animal"s);
auto name = "none"s;
return __Animal__<decltype(name)>(name);
};
;

auto main() -> decltype(0);

