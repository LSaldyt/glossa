#include "../std/std.hpp"

int main()
{
    std::function<int(int)> increment = [](int i){return i + 1;};
    Sequence<int> s(0, 10, increment);
    for (auto i : s)
    {
        print(i);
    }
    /*
    while(s)
    {
        print(*s);
        ++s;
    }
    */
}
