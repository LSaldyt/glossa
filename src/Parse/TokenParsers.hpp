#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"

namespace Parse
{
    using namespace Match;
    using SymbolicTokenParser  = std::function<TokenResult(SymbolicTokens)>;
    using SymbolicTokenParsers = std::vector<SymbolicTokenParser>;

    //Convert a standard parseFunction to one that parses Tokens
    SymbolicTokenParser subTypeParser(MatchFunction parser);
    SymbolicTokenParser typeParser(MatchFunction parser);
    SymbolicTokenParser dualTypeParser(MatchFunction typeParserFunc, MatchFunction subTypeParserFunc, bool byType);
}

