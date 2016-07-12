#include "Compiler.hpp"

namespace Compiler
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

    template <typename T>
    std::vector<std::tuple<T, T>> toPairs(std::vector<T> items)
    {
        std::cout << "Beginning toPairs" << std::endl;
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
        std::cout << "Ending toPairs" << std::endl;
        return pairs;
    }

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens)
    {
        std::cout << "Building Expression" << std::endl;
        Expression e;

        if (tokens.size() > 0)
        {
            e.base = tokens.begin()->value;
            auto pairs = toPairs(SymbolicTokens(tokens.begin() + 1, tokens.end())); 
            //Convert symbolic token pairs to only their symbol values
            for (auto pair : pairs)
            {
                e.extensions.push_back(std::make_tuple(std::get<0>(pair).value,
                                                       std::get<1>(pair).value));
            }
            tokens = SymbolicTokens(tokens.begin() + e.extensions.size() + 1, tokens.end());
            return std::make_tuple(true, e);
        }
        else
        {
            return std::make_tuple(false, e);
        }
    }

    const auto genIdent = builder<std::string>(typeParser(just("identifier")), [](SymbolicTokens tokens){return getRepr(tokens[0]);});

    StatementResult buildAssignment(SymbolicTokens& tokens)
    {
        Assignment a;
        std::vector<bool> results;
        results.push_back(bindTo<std::string>(a.identifier, genIdent, tokens));
        results.push_back(advance(subTypeParser(just("=")), tokens));
        results.push_back(bindTo<Expression>(a.value, buildExpression, tokens));
        auto result = check_results(results);
        return StatementResult(result, tokens, std::make_shared<Assignment>(a));
    }

    const auto byCommas     = subTypeParser(sepBy([](std::string s){return s == ",";}));
    const auto commaSepList = builder<std::vector<std::string>>(byCommas, getReprs);

    const auto buildStatement = [](StatementResult r){ return std::make_tuple(r.result, r.statement); };

    StatementResult buildFunction(SymbolicTokens& tokens)
    {
        Function f;
        std::vector<bool> results;
        results.push_back(bindTo<std::string>(f.identifier, genIdent, tokens));
        results.push_back(advance(subTypeParser(just("(")), tokens));
        results.push_back(bindTo<std::vector<std::string>>(f.argnames, commaSepList, tokens));
        results.push_back(advance(subTypeParser(just(")")), tokens));
        results.push_back(advance(subTypeParser(just("\n")), tokens));
        auto assignresult = buildAssignment(tokens);
        results.push_back(assignresult.result);
        f.body = assignresult.statement;
        auto result = check_results(results);
        return StatementResult(result, tokens, std::make_shared<Function>(f));
    }
}
