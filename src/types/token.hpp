#pragma once
#include "import.hpp"

/**
 * Precursor to abstract syntax elements
 * Represents an annotated term, i.e. literal int 2
 */
struct Token
{
    tools::vector<tools::string> values;
    tools::string sub_type;
    tools::string type;
    int line;
    Token(tools::vector<tools::string> set_values, tools::string set_sub_type, tools::string set_type, int set_line=0)
    {
        values   = set_values;
        sub_type = set_sub_type;
        type     = set_type;
        line     = set_line;
    }
};
using Tokens         = tools::vector<Token>;


