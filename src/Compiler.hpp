#pragma once
#include "Utilities/IO/IO.hpp"
#include "Utilities/Parse/Parse.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include "Utilities/Syntax/Token.hpp"
#include "Utilities/Lex/Lex.hpp"
#include "Utilities/Lex/Seperate.hpp"
#include "Utilities/Language.hpp"
#include "Utilities/Gen/Gen.hpp"
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

    struct Thing : public Statement
    {
        std::string identifier;
    };

    const auto identifierParser = typeParser(just("identifier"));
    const auto getRepr          = [](SymbolicTokens s){return s[0].value->representation();};

    StatementResult buildThing(SymbolicTokens& tokens)
    {
        Thing thing;
        std::tuple<bool, SymbolicTokens> result = bind(thing.identifier, from<std::string>(identifierParser, tokens, getRepr));
        return StatementResult(std::get<0>(result), std::get<1>(result), std::make_shared<Thing>(thing));
    }
}
