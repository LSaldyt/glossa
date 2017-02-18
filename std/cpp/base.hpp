#include "object.hpp"
#include <unordered_map>
#include <string>

class Base
{
protected:
    std::unordered_map<std::string, Object> __self__; // As simple as it gets 
};
