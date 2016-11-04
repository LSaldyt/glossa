#include "animal.hpp"
auto Animal::sound() -> decltype("none"s)
{
return "none"s;
};
auto Animal::f(auto x) -> decltype(x * 2)
{
return x * 2;
};
;

