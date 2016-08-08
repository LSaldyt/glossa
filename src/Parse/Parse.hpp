#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"
#include "../Syntax/Syntax.hpp"
#include "StatementResult.hpp"
#include "Typename.hpp"
#include <exception>
#include <iostream>

namespace Parse
{
    using namespace Syntax;
    using namespace Match;
    //Convert a standard parseFunction to one that parses Tokens
    std::function<TokenResult(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser);
    std::function<TokenResult(std::vector<SymbolicToken>)> typeParser(ParseFunction parser);
    std::function<TokenResult(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);

    using SymbolicTokenParser = std::function<TokenResult(SymbolicTokens)>;
    using StatementParser     = std::function<StatementResult(SymbolicTokens)>;

    bool advance(SymbolicTokenParser function, SymbolicTokens& tokens);

    struct bad_bind : public std::exception 
    {
        std::string s;
        bad_bind(std::string s)
        {
            this->s = s;
        }
    };

    template < typename T >
    std::function<std::tuple<bool, T>(SymbolicTokens&)> builder (std::function<TokenResult(std::vector<SymbolicToken>)> function, std::function<T(SymbolicTokens)> converter)
    {
        std::function<std::tuple<bool, T>(SymbolicTokens&)> f = [function, converter](SymbolicTokens& tokens)
        {
            auto result = function(tokens);
            auto to_return = std::make_tuple(false, T());
            if (result.result)
            {
                tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
                to_return = std::make_tuple(true, converter(result.parsed));
            }
            return to_return;
        };
        return f;
    }

    template <typename T>
    std::vector<std::tuple<T, T>> toPairs(std::vector<T> items)
    {
        std::vector<std::tuple<T, T>> pairs;

        auto it = items.begin();
        while(it != items.end())
        {
            if(it + 1 != items.end())
            {
                pairs.push_back(std::make_tuple(*it, *(it+1)));
                it += 2; //Iterate two items at a time 
            }
            else
            {
                break;
            }
        }
        return pairs;
    }

    std::function<StatementResult(SymbolicTokens&)> statementBuilder(std::function<std::shared_ptr<Statement>(SymbolicTokens&)> builder);

    std::tuple<bool, std::shared_ptr<Statement>>              buildStatement (SymbolicTokens& tokens);
    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens);

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens);
}
