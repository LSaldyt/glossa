#include "ToStatements.hpp"

namespace Compiler
{
    //Used to create meta statements, ex functions inside of functions
    std::tuple<bool, std::shared_ptr<Statement>> buildStatement(SymbolicTokens& tokens)
    {
        for (auto statementBuilder : statementBuilders)
        {
            SymbolicTokens tokens_copy(tokens);
            auto result = statementBuilder(tokens_copy);
            if (result.result)
            {
                tokens = tokens_copy;
                return std::make_tuple(true, result.statement); 
            }
        }
        return std::make_tuple(false, std::make_shared<Statement>(Expression()));
    }

    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens)
    {
        std::vector<std::shared_ptr<Statement>> statements;
        SymbolicTokens                          tokens_copy(tokens);

        /*auto result = buildStatement(tokens_copy);
        while (std::get<0>(result))
        {
            tokens = tokens_copy;
            statements.push_back(std::get<1>(result));
            result = buildStatement(tokens_copy);
        }*/
        return std::make_tuple(true, statements); //Has to stop eventually, so always returns true
    }

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
        results.push_back(bindTo<std::vector<std::shared_ptr<Statement>>>(f.body, buildStatements, tokens));
        for (auto t : tokens)
        {
            std::cout << "Remaining " << t.sub_type << std::endl;
        }
        results.push_back(advance(subTypeParser(just("return")), tokens));
        results.push_back(bindTo<Expression>(f.return_expression, buildExpression, tokens));
        results.push_back(advance(subTypeParser(just("\n")), tokens));
        auto result = check_results(results);
        for (auto r : results)
        {
            std::cout << r << std::endl;
        }
        return StatementResult(result, tokens, std::make_shared<Function>(f));
    }
}
