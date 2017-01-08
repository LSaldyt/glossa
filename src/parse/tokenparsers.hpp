#pragma once
#include "../match/match.hpp"
#include "../types/symbolictoken.hpp"

/**
 * Collection of parsing tools for SymbolicToken types
 * Allows parsing of type, subtype, or both of a particular token
 */
namespace parse
{
    using namespace match;
    /// Overload for match result object
    using SymbolicTokenParser  = function<Result<SymbolicToken> (vector<SymbolicToken>)>;

    //Convert a standard parseFunction to one that parses Tokens
    SymbolicTokenParser subTypeParser  (string sub_type);
    SymbolicTokenParser typeParser     (string type);
    SymbolicTokenParser dualTypeParser (string type, string sub_type);

    SymbolicTokenParser
    discard
    (SymbolicTokenParser matcher);

    vector<SymbolicToken> clean(const vector<SymbolicToken>& tokens);
    // Version of many for seperating nested multi-token parsers. Unnestable
    SymbolicTokenParser
    manySeperated
    (SymbolicTokenParser matcher, bool nonempty=false);

    vector<vector<SymbolicToken>>
    reSeperate
    (const vector<SymbolicToken>& tokens);
}

