#include "main.hpp"
;

;

auto foo(auto x)
{
print(x);

};

int main()
{
print("Running Main"s);
auto a = Animal("none"s, "no sound"s);
a.make_sound();
auto d = Dog("rover"s);
d.make_sound();
foo("Hello World"s);
return 0;

};

