#pragma once
#include "import.hpp"
#include "../match/base/types.hpp"

namespace syntax
{
class Symbol;
}

struct SymbolicToken
{
    std::shared_ptr<syntax::Symbol> value;
    std::string sub_type;
    std::string type;
    SymbolicToken(std::shared_ptr<syntax::Symbol> set_value, std::string set_sub_type, std::string set_type);
};
using SymbolicTokens = std::vector<SymbolicToken>;
