#include "ToStatements.hpp"

namespace Compiler
{

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens)
    {
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

    StatementResult buildFunction(SymbolicTokens& tokens)
    {
        Function f;
        std::vector<bool> results;
        results.push_back(bindTo<std::string>(f.identifier, genIdent, tokens));
        results.push_back(advance(subTypeParser(just("(")), tokens));
        results.push_back(bindTo<std::vector<std::string>>(f.argnames, commaSepList, tokens));
        results.push_back(advance(subTypeParser(just(")")), tokens));
        results.push_back(advance(subTypeParser(just(":")), tokens));
        results.push_back(advance(subTypeParser(just("\n")), tokens));
        auto assignresult = buildAssignment(tokens);
        results.push_back(assignresult.result);
        f.body = assignresult.statement;
        auto result = check_results(results);
        return StatementResult(result, tokens, std::make_shared<Function>(f));
    }
}
