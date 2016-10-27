#include "program.hpp"
;

auto Animal::sound() -> decltype("none"s)
{
return "none"s;
};
;

auto Dog::sound() -> decltype("bark"s)
{
return "bark"s;
};
;

auto main() -> decltype(0)
{
if (true){ while ( 1 < 2 ){ print("Hello World"s);
 };
} else { };
return 0;
};

