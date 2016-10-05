#pragma once
#include "../Match/Match.hpp"
#include "../Types/SymbolicToken.hpp"

namespace Parse
{
    using namespace Match;
    using SymbolicTokenParser  = function<Result<SymbolicToken>(SymbolicTokens)>;
    using SymbolicTokenParsers = vector<SymbolicTokenParser>;

    //Convert a standard parseFunction to one that parses Tokens
    SymbolicTokenParser subTypeParser  (string sub_type);
    SymbolicTokenParser typeParser     (string type);
    SymbolicTokenParser dualTypeParser (string type, string sub_type);

    function<Result<SymbolicToken>(vector<SymbolicToken>)>
    discard
    (function<Result<SymbolicToken>(vector<SymbolicToken>)> matcher);

    vector<SymbolicToken> clean(const vector<SymbolicToken>& tokens);
    // Version of many for seperating nested multi-token parsers. Unnestable
    function<Result<SymbolicToken>(vector<SymbolicToken>)>
    manySeperated
    (function<Result<SymbolicToken>(vector<SymbolicToken>)> matcher);

    vector<vector<SymbolicToken>>
    reSeperate
    (const vector<SymbolicToken>& tokens);
}

