#include "object.hpp"

namespace std
{

string to_string(Object o)
{
    return o.__str__();
}

}

std::ostream& operator<<(std::ostream& os, Object& obj)
{
    std::cout << obj.__str__();
    return os;
}

Object operator+(Object lhs, const Object& rhs)
{
    return lhs.__add__(rhs);
}

Object operator-(Object lhs, const Object& rhs)
{
    return lhs.__sub__(rhs);
}

Object operator/(Object lhs, const Object& rhs)
{
    return lhs.__div__(rhs);
}

Object operator*(Object lhs, const Object& rhs)
{
    return lhs.__mul__(rhs);
}
