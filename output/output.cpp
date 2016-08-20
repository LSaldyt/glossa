#include "std/std.hpp"
int main(){
const auto f = [=](auto x){ return x = f(42) + 5; };
}
