#include "std/std.hpp"
int main(){
print("hello world");
const auto bar = [=](auto y){print("Creating meta-function foo");const auto foo = [=](auto x){print("running foo with values:");print(x);print(y); return x; }; return foo; };
bar(2);
}
