#pragma once
#include "../Match/Match.hpp"
#include "../Types/SymbolicToken.hpp"

namespace Parse
{
    using namespace Match;
    using SymbolicTokenParser  = std::function<Result<SymbolicToken>(SymbolicTokens)>;
    using SymbolicTokenParsers = std::vector<SymbolicTokenParser>;

    //Convert a standard parseFunction to one that parses Tokens
    SymbolicTokenParser subTypeParser  (std::string sub_type);
    SymbolicTokenParser typeParser     (std::string type);
    SymbolicTokenParser dualTypeParser (std::string type, std::string sub_type);

    std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)>
    discard
    (std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)> matcher);

    std::vector<SymbolicToken> clean(const std::vector<SymbolicToken>& tokens);
    // Version of many for seperating nested multi-token parsers. Unnestable
    std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)>
    manySeperated
    (std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)> matcher);

    std::vector<std::vector<SymbolicToken>>
    reSeperate
    (const std::vector<SymbolicToken>& tokens);
}

