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



    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens)
    {
        std::vector<std::shared_ptr<Statement>> statements;
        SymbolicTokens                          tokens_copy(tokens);

        auto result = buildStatement(tokens_copy);
        while (std::get<0>(result))
        {
            std::cout << "Built statement" << std::endl;
            tokens = tokens_copy;
            statements.push_back(std::get<1>(result));
            result = buildStatement(tokens_copy);
        }
        return std::make_tuple(true, statements); //Has to stop eventually, so always returns true
    }


    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens)
    {
        const auto validType = typeParser(allOf({anyOf({just("type"), just("identifier")}), inverse(just("keyword"))}));
        Expression e;
        const auto validPair = [validType](std::tuple<SymbolicToken, SymbolicToken> pair)
        {
            auto result_1 = typeParser(just("operator"))(SymbolicTokens(1, std::get<0>(pair)));
            auto result_2 = validType(SymbolicTokens(1, std::get<1>(pair)));
            return bool(result_1.result && result_2.result); 
        };

        if (tokens.size() > 0)
        {
            if(!validType(SymbolicTokens(1, *tokens.begin())).result)
            {
                return std::make_tuple(false, e);
            }
            e.base = tokens.begin()->value;
            auto pairs = toPairs<SymbolicToken>(SymbolicTokens(tokens.begin() + 1, tokens.end())); 
            //Convert symbolic token pairs to only their symbol values
            for (auto pair : pairs)
            {
                if (validPair(pair))
                {
                    e.extensions.push_back(std::make_tuple(std::get<0>(pair).value,
                                                           std::get<1>(pair).value));
                }
                else
                {
                    break;
                }
            }
            tokens = SymbolicTokens(tokens.begin() + e.extensions.size() + 1, tokens.end());
            advance(subTypeParser(just("\n")), tokens);
            return std::make_tuple(true, e);
        }
        else
        {
            return std::make_tuple(false, e);
        }
    }

    std::function<StatementResult(SymbolicTokens&)> statementBuilder(std::function<std::shared_ptr<Statement>(SymbolicTokens&)> builder)
    {
       return [builder](SymbolicTokens& tokens)
       {
           try{
               auto statement = builder(tokens);
               return StatementResult(true, tokens, statement);
           }
           catch (const bad_bind& e){
               std::cout << e.s << std::endl;
               return StatementResult(false, tokens, std::make_shared<Statement>(Statement())); //Empty statement
           }
       };
    }
}
