#include "main.hpp"
;

;

auto main() -> decltype(0)
{
print("Running Main"s);
auto a = Animal();
a.make_sound();
auto d = Dog("rover"s);
d.make_sound();
return 0;
};

