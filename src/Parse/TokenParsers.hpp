#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"

namespace Parse
{
    using namespace Match;
    using SymbolicTokenParser = std::function<TokenResult(SymbolicTokens)>;
    //Convert a standard parseFunction to one that parses Tokens
    std::function<TokenResult(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser);
    std::function<TokenResult(std::vector<SymbolicToken>)> typeParser(ParseFunction parser);
    std::function<TokenResult(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);
}

