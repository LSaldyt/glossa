#include "Parse.hpp"

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
        
        bool any = false;

        auto result = buildStatement(tokens_copy);
        while (std::get<0>(result))
        {
            any = true;
            std::cout << "Built statement" << std::endl;
            tokens = tokens_copy;
            statements.push_back(std::get<1>(result));
            result = buildStatement(tokens_copy);
        }

        std::cout << any << std::endl;
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
