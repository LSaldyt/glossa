#include "std/std.hpp"
int main(){
print("hello world");
const auto foo = [=](auto x, auto y){ return x + y; };
}
