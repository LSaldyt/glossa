#include "std/std.hpp"
int main(){
const auto f = [=](auto x){const auto g = [=](auto y){ return y; }; return g; };
print(2);
const auto x = 42;
}
