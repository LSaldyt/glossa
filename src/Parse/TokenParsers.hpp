#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"

namespace Parse
{
    using namespace Match;
    using SymbolicTokenParser = std::function<TokenResult(SymbolicTokens)>;

    //Convert a standard parseFunction to one that parses Tokens
    SymbolicTokenParser subTypeParser(ParseFunction parser);
    SymbolicTokenParser typeParser(ParseFunction parser);
    SymbolicTokenParser dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);
}

