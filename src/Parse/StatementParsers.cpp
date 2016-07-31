#include "StatementParsers.hpp"

namespace Parse 
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
}
