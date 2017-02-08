#include <chrono>
#include <iostream>

auto time()
{
    return std::chrono::high_resolution_clock::now();
}

std::ostream& operator <<(std::ostream& os, const std::chrono::duration<long int, std::ratio<1l, 1000000000l>>& t)
{
    os << std::chrono::duration_cast<std::chrono::duration<float>>(t).count();
    return os;
}
