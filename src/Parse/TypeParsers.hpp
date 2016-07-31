#include "Parse.hpp"

namespace Parse
{

    //Helper functions

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


    //

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

    const auto parseOp = [](std::string opname){
        return subTypeParser(just(opname));
    };
}
