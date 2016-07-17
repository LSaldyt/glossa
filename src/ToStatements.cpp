#include "ToStatements.hpp"

namespace Compiler
{
    //Used to create meta statements, ex functions inside of functions
    std::tuple<bool, std::shared_ptr<Statement>> buildStatement(SymbolicTokens& tokens)
    {
        for (auto statementBuilder : statementBuilders)
        {
            std::cout << "Attempting to build statement" << std::endl;
            SymbolicTokens tokens_copy(tokens);
            auto result = statementBuilder(tokens_copy);
            if (result.result)
            {
                tokens = tokens_copy;
                return std::make_tuple(true, result.statement); 
            }
            else
            {
                std::cout << "Failed.. Trying next statement type" << std::endl;
            }
        }
        std::cout << "Statement building failed" << std::endl;
        return std::make_tuple(false, std::make_shared<Statement>(Expression()));
    }

    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens)
    {
        std::vector<std::shared_ptr<Statement>> statements;
        SymbolicTokens                          tokens_copy(tokens);

        auto result = buildStatement(tokens_copy);
        while (std::get<0>(result))
        {
            std::cout << "Built statement" << std::endl;
            tokens = tokens_copy;
            statements.push_back(std::get<1>(result));
            result = buildStatement(tokens_copy);
        }
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

    StatementResult buildAssignment(SymbolicTokens& tokens)
    {
        Assignment a;
        std::vector<bool> results;
        results.push_back(bindTo<std::string>(a.identifier, genIdent, tokens));
        results.push_back(advance(subTypeParser(just("=")), tokens));
        results.push_back(bindTo<Expression>(a.value, buildExpression, tokens));
        results.push_back(advance(subTypeParser(just("\n")), tokens));
        auto result = check_results(results);
        return StatementResult(result, tokens, std::make_shared<Assignment>(a));
    }

    StatementResult buildFunction(SymbolicTokens& tokens)
    {
        std::cout << "Building Function" << std::endl;
        for (auto t : tokens)
        {
            std::cout << "Given: " << t.type << " " << t.sub_type << std::endl;
        }
        Function f;
        std::vector<bool> results;
        results.push_back(bindTo<std::string>(f.identifier, genIdent, tokens));
        results.push_back(advance(subTypeParser(just("(")), tokens));
        results.push_back(bindTo<std::vector<std::string>>(f.argnames, commaSepList, tokens));
        results.push_back(advance(subTypeParser(just(")")), tokens));
        results.push_back(advance(subTypeParser(just(":")), tokens));
        results.push_back(advance(subTypeParser(just("\n")), tokens));
        std::cout << "Building Function Body" << std::endl;
        auto result = check_results(results);
        if (result)
        {
            results.push_back(bindTo<std::vector<std::shared_ptr<Statement>>>(f.body, buildStatements, tokens));
            results.push_back(advance(subTypeParser(just("return")), tokens));
            results.push_back(bindTo<Expression>(f.return_expression, buildExpression, tokens));
            result = check_results(results);
        }
        for(auto r : results)
        {
            std::cout << r << std::endl;
        }
        if (result)
        {
            std::cout << "Function successfully built" << std::endl;
        }
        return StatementResult(result, tokens, std::make_shared<Function>(f));
    }
}
