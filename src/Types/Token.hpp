#pragma once
#include "Import.hpp"

struct Token
{
    std::vector<std::string> values;
    std::string sub_type;
    std::string type;
    Token(std::vector<std::string> set_values, std::string set_sub_type, std::string set_type)
    {
        values   = set_values;
        sub_type = set_sub_type;
        type     = set_type;
    }
};
using Tokens         = std::vector<Token>;


