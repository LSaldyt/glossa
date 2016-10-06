#include <ostream>

struct string_view
{
    char const* data;
    size_t size;
};

inline ostream& operator<<(ostream& o, string_view const& s)
{
    return o.write(s.data, s.size);
}

template<class T>
constexpr string_view get_name()
{
    char const* p = __PRETTY_FUNCTION__;
    while (*p++ != '=');
    for (; *p == ' '; ++p);
    char const* p2 = p;
    int count = 1;
    for (;;++p2)
    {
        switch (*p2)
        {
            case '[':
            ++count;
            break;
            case ']':
            --count;
            if (!count)
            return {p, size_t(p2 - p)};
        }
    }
    return {};
}
