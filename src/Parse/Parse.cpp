#include "Parse.hpp"

namespace Parse
{
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser)
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
                return TokenResult<SymbolicToken>(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + result.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult<SymbolicToken>(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }

    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> typeParser(ParseFunction parser)
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
                return TokenResult<SymbolicToken>(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + result.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + result.parsed.size(), tokens.end()));
            }
            return TokenResult<SymbolicToken>(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }

    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType)
    {
        auto parseTokens = [typeParserFunc, subTypeParserFunc, byType](std::vector<SymbolicToken> tokens)
        {
            auto typeResult    = typeParser    (typeParserFunc)    (tokens);
            auto subTypeResult = subTypeParser (subTypeParserFunc) (tokens);
            if(typeResult.result && subTypeResult.result)
            {
                if(byType)
                {
                return TokenResult<SymbolicToken>(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + typeResult.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + typeResult.parsed.size(), tokens.end()));
                }
                else
                {
                return TokenResult<SymbolicToken>(true,
                        std::vector<SymbolicToken>(tokens.begin(), tokens.begin() + subTypeResult.parsed.size()),
                        std::vector<SymbolicToken>(tokens.begin() + subTypeResult.parsed.size(), tokens.end()));
                }
            }
            return TokenResult<SymbolicToken>(false, std::vector<SymbolicToken>(), tokens);
        };
        return parseTokens;
    }

    //inOrder for tokenTypes
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> inOrderTokenParser(std::vector<std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)>> parsers)
    {
        return [parsers](std::vector<SymbolicToken> original_tokens)
        {
            std::vector<SymbolicToken> parsed;
            std::vector<SymbolicToken> tokens = original_tokens;

            for (auto parser : parsers)
            {
                auto parse_result = parser(tokens);
                if(parse_result.result)
                {
                    parsed.insert( parsed.end(), parse_result.parsed.begin(), parse_result.parsed.end() );
                    tokens  = parse_result.remaining;
                }
                else
                {
                    return TokenResult<SymbolicToken>(false, std::vector<SymbolicToken>(), tokens);
                }
            }
            return TokenResult<SymbolicToken>(true, parsed, tokens);
        };
    }

    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> constructDualTypeParser(std::vector<std::tuple<ParseFunction, ParseFunction>> parser_pairs, bool byType)
    {
        auto dual_types = std::vector<std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)>>();
        dual_types.reserve(parser_pairs.size());
        for (auto parser_pair : parser_pairs)
        {
            dual_types.push_back(dualTypeParser(std::get<0>(parser_pair), std::get<1>(parser_pair), byType));
        }
        return inOrderTokenParser(dual_types);
    }

    bool advance(SymbolicTokenParser function, SymbolicTokens& tokens)
    {
        auto result = function(tokens);
        if (!result.result)
        {
            return false;
        }
        else
        {
            tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
            return true;
        }
    }

}
