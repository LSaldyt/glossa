#include "Compiler.hpp"

namespace Compiler
{
    using namespace Parse;
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

    const auto commaSepList = builder<std::vector<std::string>>(subTypeParser(sepBy(just(","))), getReprs);

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens);

    const auto genIdent = builder<std::string>(typeParser(just("identifier")), [](SymbolicTokens tokens){return getRepr(tokens[0]);});
    const auto buildStatement = [](StatementResult r){ return std::make_tuple(r.result, r.statement); };

    StatementResult buildAssignment(SymbolicTokens& tokens);
    StatementResult buildFunction(SymbolicTokens& tokens);
}
