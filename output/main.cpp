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
for (auto& n : range(10)){ auto i = n;
while ( i > 0 ){ i = i - 1;
if (i % 2 == 0){ print("EVEN"s);
} else { print("ODD"s);
};
print(i);
 };
 };
return 0;
};

