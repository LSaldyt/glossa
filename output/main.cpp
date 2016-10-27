#include "main.hpp"
;

;

auto main() -> decltype(0)
{
print("Running Main"s);
auto dogs = std::vector<decltype(Dog())>({Dog(), Dog()});
for (auto& dog : dogs){ print(dog.sound());
 };
for (auto& n : std::vector<decltype(10)>({10, 5})){ auto i = n;
while ( i > 0 ){ i = i - 1;
if (i % 2 == 0){ print("EVEN"s);
} else { print("ODD"s);
};
print(i);
 };
 };
return 0;
};

