#include <unordered_map>

template <typename T1, typename T2>
T2& get(std::unordered_map<T1, T2>& d, const T1& t1)
{
    return d[t1];
}

template <typename T1, typename T2>
void insert(std::unordered_map<T1, T2>& d, const T1& t1, const T2& t2)
{
    d[t1] = t2;
}

template <typename T1, typename T2>
T1 first(std::pair<T1, T2>& kv)
{
    return kv.first;
}

template <typename T1, typename T2>
T2 second(std::pair<T1, T2>& kv)
{
    return kv.second;
}
