#include "std/std.hpp"
int main(){
const auto f = [=](auto x){print(x); return x * 8; };
const auto x = 5 * f(42);;
const auto y = 3 + 2;
}
