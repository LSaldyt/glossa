int x = 42;
int y = 32 + x;
template< typename Tx >
auto f(Tx x) -> decltype(x * x)
{
    return x * x;
}

