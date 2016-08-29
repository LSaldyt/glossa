#include "std/std.hpp"
int main(){
const auto f = [=](auto x){print(x); return x; };
}
