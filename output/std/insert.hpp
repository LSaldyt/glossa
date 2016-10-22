#include <unordered_map>

template <typename T1, typename T2>
void insert(std::unordered_map<T1, T2>& d, const T1& t1, const T2& t2)
{
    d[t1] = t2;
}

