#pragma once
#include "import.hpp"

namespace syntax
{
class Symbol;
}

struct SymbolicToken
{
    std::shared_ptr<syntax::Symbol> value;
    std::string text;
    std::string sub_type;
    std::string type;
    SymbolicToken(std::shared_ptr<syntax::Symbol> set_value, std::string set_sub_type, std::string set_type, std::string set_text);
};
using SymbolicTokens = std::vector<SymbolicToken>;
