#pragma once
#include "tools/tools.hpp"
#include "Syntax/Statements.hpp"
#include "Lex/Lex.hpp"
#include "Lex/Seperate.hpp"
#include "Lex/Language.hpp"
#include <iostream>
#include <tuple>
#include "Types/Symbolize.hpp"
#include "Parse/Grammar/grammar.hpp"

namespace Compiler
{
    using namespace Parse;
    using namespace Lex;
    using namespace Syntax;
    using namespace Grammar;
    using namespace tools;

    std::vector<Tokens> tokenPass(std::vector<std::string>, const Language&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);
}
