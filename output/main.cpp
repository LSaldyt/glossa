#include "main.hpp"
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
print("Running Main"s);
auto i = 10;
while ( i > 0 ){ i = i - 1;
print(i);
 };
return 0;
};

