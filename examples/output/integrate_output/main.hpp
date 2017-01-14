#include "../std/std.hpp"
template <typename T_x>
auto f (T_x x)
{
    return x * x - x;
}
template <typename T_a,typename T_b,typename T_N>
auto integrate_f (T_a a,T_b b,T_N N)
{
    auto s = 0;
    auto dx = (b - a) / N;
    for (auto i : range(N))
    {
        s = s + f(a + i * dx);
    };
    return s * dx;
}

