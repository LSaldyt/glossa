#pragma once
#include "../Match/Match.hpp"

namespace Parse
{
    using namespace Syntax;
    //Convert a standard parseFunction to one that parses Tokens
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> typeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);

    //inOrder for tokenTypes
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> inOrderTokenParser(std::vector<std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)>> parsers);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> constructDualTypeParser(std::vector<std::tuple<ParseFunction, ParseFunction>> parser_pairs, bool byType);
}
