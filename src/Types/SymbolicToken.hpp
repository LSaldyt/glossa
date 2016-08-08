#pragma once
#include "../Match/Base/Types.hpp"
#include "../Syntax/Symbols.hpp"

struct SymbolicToken
{
    std::shared_ptr<Syntax::Symbol> value;
    std::string sub_type;
    std::string type;
    SymbolicToken(std::shared_ptr<Syntax::Symbol> set_value, std::string set_sub_type, std::string set_type)
    {
        value    = set_value;
        sub_type = set_sub_type;
        type     = set_type;
    }
};
using SymbolicTokens = std::vector<SymbolicToken>;
