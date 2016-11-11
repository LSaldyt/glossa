#include "main.hpp"
Animal::Animal()
{
print("Creating Animal"s);
auto __new_members = std::tuple_cat(__members, std::make_tuple('a'));
__access_map[name] = 0;;

};
auto Animal::sound() -> decltype("none"s)
{
return "none"s;
};
;

Dog::Dog()
{
print("Creating Dog"s);
auto __new_members = std::tuple_cat(__members, std::make_tuple('a'));
__access_map[name] = 0;;

};
auto Dog::sound() -> decltype("bark"s)
{
return "bark"s;
};
;

auto main() -> decltype(0)
{
print("Running Main"s);
auto a = Animal();
print(a.sound());
auto d = Dog();
print(d.sound());
return 0;
};

