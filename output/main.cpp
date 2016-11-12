#include "main.hpp"
;

auto main() -> decltype(0)
{
print("Running Main"s);
auto a = Animal();
print(a.sound());
return 0;
};

