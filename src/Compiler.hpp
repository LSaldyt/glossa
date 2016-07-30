#pragma once
#include "IO/IO.hpp"
#include "Parse/Parse.hpp"
#include "Syntax/Symbols.hpp"
#include "Syntax/Token.hpp"
#include "Lex/Lex.hpp"
#include "Lex/Seperate.hpp"
#include "Lex/Language.hpp"
#include "Gen/Gen.hpp"
#include <iostream>
#include <tuple>
#include "Parse/ToStatements.hpp"

namespace Compiler
{
    using namespace Parse;
    using namespace Lex;
    using namespace Syntax;
    using namespace Gen;

    std::vector<Tokens> tokenPass(std::vector<std::string>, const Language&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);
}
