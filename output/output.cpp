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

if (b | c()){ f(g(x) * y);
if (c()){ auto a = 2;
} else { };
} else { h(n);
};

print("hello world"s);

print(f(g(h(2))) * h(200));

for (auto& i : range(10)){ print(i);
 };

auto v = std::vector<decltype(2)>({2, 3});

append(v, 4);

for (auto& i : v){ print(i);
 };

auto d = std::unordered_map<decltype("a"s), decltype(1)>{ 
{"a"s, 1}
 };

insert(d, "b"s, 2);

for (auto& kv : d){ print(first(kv));
print(second(kv));
 };

}
