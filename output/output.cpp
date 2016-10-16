#include "std/std.hpp"
int main(){
auto f = [=](auto a){return a * 2; };
auto g = [=](auto x){return x; };
auto h = [=](auto x){return x * x; };
auto x = 2;
x = 4;
auto y = 3;
auto n = 10;
auto c = [=](){return true; };
auto b = true;
if (b | c()){ f(g(x) * y);auto a = 2;} else { h(n);};
print("hello world");
print(f(g(h(2))) * h(200));
}
