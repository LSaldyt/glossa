#pragma once
#include "Import.hpp"

struct Token
{
    std::string value;
    std::string sub_type;
    std::string type;
    Token(std::string set_value, std::string set_sub_type, std::string set_type)
    {
        value    = set_value;
        sub_type = set_sub_type;
        type     = set_type;
    }
};
using Tokens         = std::vector<Token>;


