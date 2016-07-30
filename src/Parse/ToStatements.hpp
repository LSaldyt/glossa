#include "Parse.hpp"
#include "../Syntax/Symbols.hpp"
#include "../Syntax/Token.hpp"
#include <iostream>
#include <tuple>

namespace Parse
{
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

    const auto check_results = [](std::vector<bool> results)
    {
        for (auto r : results)
        {
            if (! r)
            {
                return false;
            }
        }
        return true;
    };

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

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens);
    const auto buildAssignment = statementBuilder([=](SymbolicTokens& tokens){
        Assignment a;
        bindTo<std::string>(a.identifier, genIdent, tokens);
        advance(subTypeParser(just("=")), tokens);
        bindTo<Expression>(a.value, buildExpression, tokens);
        advance(subTypeParser(just("\n")), tokens);
        return std::make_shared<Assignment>(a);
        });



    StatementResult buildFunction(SymbolicTokens& tokens);
    StatementResult buildFunctionCall(SymbolicTokens& tokens);

    const std::vector<std::function<StatementResult(SymbolicTokens&)>> statementBuilders = 
    {
        buildAssignment, buildFunction 
    };

    std::tuple<bool, std::shared_ptr<Statement>>              buildStatement (SymbolicTokens& tokens);
    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens);
}
