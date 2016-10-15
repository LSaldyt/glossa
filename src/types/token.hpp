#pragma once
#include "import.hpp"

struct Token
{
    tools::vector<tools::string> values;
    tools::string sub_type;
    tools::string type;
    Token(tools::vector<tools::string> set_values, tools::string set_sub_type, tools::string set_type)
    {
        values   = set_values;
        sub_type = set_sub_type;
        type     = set_type;
    }
};
using Tokens         = tools::vector<Token>;


