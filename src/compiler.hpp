#pragma once
#include "tools/tools.hpp"
#include "syntax/statements.hpp"
#include "lex/lex.hpp"
#include "lex/seperate.hpp"
#include "lex/language.hpp"
#include "types/symbolize.hpp"
#include "grammar/grammar.hpp"

namespace compiler
{
    using namespace parse;
    using namespace lex;
    using namespace syntax;
    using namespace grammar;
    using namespace tools;

    std::vector<Tokens> tokenPass(std::vector<std::string>, const Language&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);
}
