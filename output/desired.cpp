#include <iostream>

template <typename T>
void print(T x)
{
    auto bar = [](auto x){ std::cout << x << std::endl; }; // imaginary syntax
    bar(x);
}

int main()
{
    print("hello world");
}
