#include "std/std.hpp"

const int x = 42;
const int y = 32 + x;
template< typename Tx >
auto f(Tx x) -> decltype(x * x)
{
    return x * x;
}

int main()
{
    print(2 + 2);
    return 0;
}
