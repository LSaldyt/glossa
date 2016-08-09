#include "TokenParsers.hpp"

namespace Parse
{

    SymbolicTokenParser subTypeParser(ParseFunction parser)
    {
        auto parseTokens = [parser](SymbolicTokens tokens)
        {
            auto terms = Terms();
            terms.reserve(tokens.size());
            for (auto token : tokens)
            {
                terms.push_back(token.sub_type);
            }

            auto result = parser(terms);
            if(result.result)
            {
                return TokenResult(true,
                        SymbolicTokens(tokens.begin(), tokens.begin() + result.parsed.size()),
                        SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult(false, SymbolicTokens(), tokens);
        };
        return parseTokens;
    }

    SymbolicTokenParser typeParser(ParseFunction parser)
    {
        auto parseTokens = [parser](SymbolicTokens tokens)
        {
            auto terms = Terms();
            terms.reserve(tokens.size());
            for (auto token : tokens)
            {
                terms.push_back(token.type);
            }

            auto result = parser(terms);
            if(result.result)
            {
                return TokenResult(true,
                        SymbolicTokens(tokens.begin(), tokens.begin() + result.parsed.size()),
                        SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult(false, SymbolicTokens(), tokens);
        };
        return parseTokens;
    }

    SymbolicTokenParser dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType)
    {
        auto parseTokens = [typeParserFunc, subTypeParserFunc, byType](SymbolicTokens tokens)
        {
            auto typeResult    = typeParser    (typeParserFunc)    (tokens);
            auto subTypeResult = subTypeParser (subTypeParserFunc) (tokens);
            if(typeResult.result && subTypeResult.result)
            {
                if(byType)
                {
                return TokenResult(true,
                        SymbolicTokens(tokens.begin(), tokens.begin() + typeResult.parsed.size()),
                        SymbolicTokens(tokens.begin() + typeResult.parsed.size(), tokens.end()));
                }
                else
                {
                return TokenResult(true,
                        SymbolicTokens(tokens.begin(), tokens.begin() + subTypeResult.parsed.size()),
                        SymbolicTokens(tokens.begin() + subTypeResult.parsed.size(), tokens.end()));
                }
            }
            return TokenResult(false, SymbolicTokens(), tokens);
        };
        return parseTokens;
    }
}
