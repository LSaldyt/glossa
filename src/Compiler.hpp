#pragma once
#include "IO/IO.hpp"
#include "Parse/Parse.hpp"
#include "Syntax/Syntax.hpp"
#include "Lex/Lex.hpp"
#include "Lex/Seperate.hpp"
#include "Lex/Language.hpp"
#include <iostream>
#include <tuple>
#include "Parse/StatementParsers.hpp"

namespace Compiler
{
    using namespace Parse;
    using namespace Lex;
    using namespace Syntax;

    std::vector<Tokens> tokenPass(std::vector<std::string>, const Language&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);
}
