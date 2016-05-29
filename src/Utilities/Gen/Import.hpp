#pragma once
#include "../Syntax/Statements.hpp"
#include "../Parse/Parse.hpp"

namespace Gen
{
    using namespace Syntax;
    using SymbolicTokenParser      = std::function<TokenResult<SymbolicToken>(SymbolicTokens)>;
    using SymbolicStatementParser  = std::tuple<SymbolicTokenParser, StatementGenerator>;
    using SymbolicStatementParsers = std::vector<SymbolicStatementParser>;
}
