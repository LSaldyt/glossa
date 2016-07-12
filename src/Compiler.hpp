#pragma once
#include "IO/IO.hpp"
#include "Parse/Parse.hpp"
#include "Syntax/Symbols.hpp"
#include "Syntax/Token.hpp"
#include "Lex/Lex.hpp"
#include "Lex/Seperate.hpp"
#include "Language.hpp"
#include "Gen/Gen.hpp"
#include <iostream>
#include <tuple>

namespace Compiler
{
    using namespace Parse;
    using namespace Lex;
    using namespace Syntax;
    using namespace Gen;

    std::vector<Tokens> tokenPass(std::vector<std::string>, const Language&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);

    const auto makeTypeParserFromStrings = [](std::vector<std::string> strings)
    {
        using namespace Parse;
        using namespace Syntax;
        return typeParser(inOrder(justFrom(strings)));
    };

    const auto makeTypeParser = [](std::vector<ParseFunction> functions)
    {
        using namespace Parse;
        using namespace Syntax;
        return typeParser(inOrder(functions));
    };
}
