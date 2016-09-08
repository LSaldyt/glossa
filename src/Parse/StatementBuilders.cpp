#include "StatementBuilders.hpp"

namespace Parse 
{
    //Used to create meta statements, ex functions inside of functions
    std::tuple<bool, std::shared_ptr<Statement>> buildStatement(SymbolicTokens& tokens)
    {
        for (auto statementBuilder : statementBuilders)
        {
            SymbolicTokens tokens_copy(tokens);
            std::cout << "Attempting to build statement" << std::endl;
            auto result = statementBuilder(tokens_copy);
            if (result.result)
            {
                std::cout << "Successfully build statement, returning" << std::endl;
                tokens = tokens_copy;
                return std::make_tuple(true, result.statement); 
            }
            else
            {
                std::cout << "Failed.. Trying next statement type" << std::endl;
            }
        }
        std::cout << "Statement building failed" << std::endl;
        return std::make_tuple(false, std::make_shared<Statement>(Statement()));
    }

    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens)
    {
        std::vector<std::shared_ptr<Statement>> statements;
        SymbolicTokens                          tokens_copy(tokens);
        
        bool any = false;

        auto result = buildStatement(tokens_copy);
        while (std::get<0>(result))
        {
            any = true;
            std::cout << "Built statement" << std::endl;
            tokens = tokens_copy;
            statements.push_back(std::get<1>(result));
            result = buildStatement(tokens_copy);
        }

        std::cout << any << std::endl;
        return std::make_tuple(true, statements); //Has to stop eventually, so always returns true
    }

    std::tuple<bool, std::shared_ptr<Symbol>> buildValue(SymbolicTokens& tokens)
    {
        std::cout << "Building Value" << std::endl;
        SymbolicTokens tokens_copy(tokens);
        //const auto value = typeParser("literal");
        const auto value = anyOf<SymbolicToken>({typeParser("literal"), typeParser("identifier")});

        if (tokens_copy.size() > 0)
        {
            auto result = buildFunctionCall(tokens_copy); // Will eventually be any complex R-value (Which, right now, is only function calls)
            if (result.result)
            {
                tokens = tokens_copy;
                return std::make_tuple(true, result.statement);
            }
            else
            {
                tokens_copy = tokens;
            }
            auto valresult = value(tokens_copy);
            if (valresult.result)
            {
                tokens = valresult.remaining;
                return std::make_tuple(true, valresult.consumed[0].value);
            }
        }
        return std::make_tuple(false, std::make_shared<Symbol>(Symbol()));
    }

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens)
    {
        Expression e;
        SymbolicTokens tokens_copy(tokens);
        bool parsing = tokens.size() > 0;
        if (parsing)
        {
            auto result = buildValue(tokens_copy);
            if (std::get<0>(result))
            {
                tokens = tokens_copy;
                e.base = std::get<1>(result);
            }
            else
            {
                return std::make_tuple(false, e);
            }
        }
        parsing = tokens.size() > 1; // An operator and single-token literal, at least
        while (parsing)
        {
            std::cout << "Parsing extensions" << std::endl;
            auto first  = SymbolicTokens(tokens_copy.begin(), tokens_copy.begin() + 1);
            tokens_copy = SymbolicTokens(tokens_copy.begin() + 1, tokens_copy.end());

            auto opresult  = typeParser("operator")(first);
            auto valresult = buildValue(tokens_copy);

            bool result(opresult.result && std::get<0>(valresult));

            if (result)
            {
                tokens = tokens_copy;
                e.extensions.push_back(
                        std::make_tuple(opresult.consumed[0].value,
                                        std::get<1>(valresult)));
                std::cout << opresult.consumed[0].value->representation() << std::endl;
                std::cout << std::get<1>(valresult)->representation() << std::endl;
                parsing = tokens.size() > 1; 
            }
            else
            {
                parsing = false;
            }
        }
        return std::make_tuple(true, e);
    }
}
