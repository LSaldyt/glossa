#include "Parse.hpp"

namespace Parse
{
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
