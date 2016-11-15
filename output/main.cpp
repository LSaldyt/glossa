#include "main.hpp"
int main(int argc, char ** argv){
std::vector<std::string> __args__; 
for (int i = 0; i < argc; i++)
{__args__.push_back(argv[i]);}
    print("Running Main"s);
    auto a = Animal("none"s, "no sound"s);
    a.make_sound();
    auto d = Dog("rover"s, "goldenretriever"s);
    d.make_sound();
    print(foo(true));
    for (auto& arg : __args__){ print(arg);
 };
    return 0;
}
;

