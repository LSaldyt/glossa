#include "TokenParsers.hpp"

namespace Parse
{

    std::function<TokenResult(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser)
    {
        auto parseTokens = [parser](std::vector<SymbolicToken> tokens)
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
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + result.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }

    std::function<TokenResult(std::vector<SymbolicToken>)> typeParser(ParseFunction parser)
    {
        auto parseTokens = [parser](std::vector<SymbolicToken> tokens)
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
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + result.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }

    std::function<TokenResult(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType)
    {
        auto parseTokens = [typeParserFunc, subTypeParserFunc, byType](std::vector<SymbolicToken> tokens)
        {
            auto typeResult    = typeParser    (typeParserFunc)    (tokens);
            auto subTypeResult = subTypeParser (subTypeParserFunc) (tokens);
            if(typeResult.result && subTypeResult.result)
            {
                if(byType)
                {
                return TokenResult(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + typeResult.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + typeResult.parsed.size(), tokens.end()));
                }
                else
                {
                return TokenResult(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + subTypeResult.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + subTypeResult.parsed.size(), tokens.end()));
                }
            }
            return TokenResult(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }
}
