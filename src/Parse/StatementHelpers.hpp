#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"
#include "../Syntax/Statements.hpp"
#include "StatementResult.hpp"
#include "Typename.hpp"
#include <exception>
#include <iostream>
#include "TokenParsers.hpp"

namespace Parse
{
    using namespace Syntax;
    using namespace Match;

    using StatementParser = std::function<StatementResult(SymbolicTokens&)>;

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
    std::function<std::tuple<bool, T>(SymbolicTokens&)> builder 
    (std::function<TokenResult(std::vector<SymbolicToken>)> function, std::function<T(SymbolicTokens)> converter)
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

    StatementParser statementBuilder(std::function<std::shared_ptr<Statement>(SymbolicTokens&)> builder);

    std::tuple<bool, std::shared_ptr<Statement>>              buildStatement (SymbolicTokens& tokens);
    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens);

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens);


    const auto identifierParser = typeParser(just("identifier"));
    const auto getRepr          = [](SymbolicToken s){return s.value->representation();};
    const auto getReprs         = [](SymbolicTokens tokens)
        {
            std::vector<std::string> strings;
            for (auto t : tokens)
            {
                strings.push_back(getRepr(t));
            }
            return strings;
        };

    const auto typeOrIdent = anyOf({just("type"), just("identifier")});
    const auto expression_parser = inOrder({
           typeOrIdent,
           many(inOrder({just("operator"), typeOrIdent}))
    });

    const auto everyOther = [](std::vector<std::string> terms)
    {
        std::vector<std::string> every_other;
        for (int i = 0; i < terms.size(); i++)
        {
            if (i % 2 == 0)
            {
                every_other.push_back(terms[i]);
            }
        }
        return every_other;
    };

    const auto compose = [](auto f, auto g)
    {
        return [f, g](auto x){ return f(g(x)); };
    };

    const auto commaSepList = builder<std::vector<std::string>>(subTypeParser(sepBy(just(","))), compose(everyOther, getReprs));
    const auto genIdent = builder<std::string>(typeParser(just("identifier")), [](SymbolicTokens tokens){return getRepr(tokens[0]);});


    template <typename T>
    std::function<void(T&, SymbolicTokens&)> createBinder(std::function<std::tuple<bool, T>(SymbolicTokens&)> typeGenerator)
    {
        return [typeGenerator](T& t, SymbolicTokens& tokens){
            auto result = typeGenerator(tokens);
            if (! std::get<0>(result))
            {
                throw bad_bind("Failed to bind to " + std::string(get_name<decltype(t)>().data)); 
            }
            else
            {
                t = std::get<1>(result);
            }
        };
    }

    const auto bindIdent      = createBinder<std::string>                             (genIdent);
    const auto bindArgnames   = createBinder<std::vector<std::string>>                (commaSepList);
    const auto bindStatements = createBinder<std::vector<std::shared_ptr<Statement>>> (buildStatements);
    const auto bindExpression = createBinder<Expression>                              (buildExpression);

    const auto subType = [](std::string opname){
        return subTypeParser(just(opname));
    };
}
