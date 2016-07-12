#pragma once
#include "../Syntax/Statements.hpp"
#include "../Parse/Parse.hpp"

namespace Gen
{
    using namespace Syntax;
    using namespace Parse;

    using SymbolicTokenParser = std::function<TokenResult<SymbolicToken>(SymbolicTokens)>;
}
