#pragma once
#include "Symbols.hpp"
#include "Token.hpp"

namespace Syntax
{
    struct Statement
    {
        virtual std::string generator(){ return ""; }
    };

    using StatementGenerator = std::function<std::shared_ptr<Statement>(SymbolicTokens)>;

    struct Assignment : public Statement
    {
        std::string identifier;
        std::shared_ptr<Symbol> value;
        std::string type;
        Assignment(std::string set_identifier,
                   std::shared_ptr<Symbol> set_value,
                   std::string set_type)
        {
            identifier = set_identifier;
            value      = set_value;
            type       = set_type;
        }

        std::string generator()
        {
            return (type + " " + identifier + " = " + value->representation() + ";");
        }
    };

    struct Function : public Statement
    {
        std::string identifier;

        std::string generator()
        {
            return "int f(int x);";
        }
    };

    const StatementGenerator FunctionGenerator = [](SymbolicTokens tokens)
    {
        return std::make_shared<Function>(Function());
    };

    const StatementGenerator AssignmentGenerator = [](SymbolicTokens tokens)
    {
        auto identifier = tokens[0].value->representation();
        auto value      = tokens[2].value;
        auto type       = tokens[2].sub_type;
        return std::make_shared<Assignment>(Assignment(identifier, value, type));
    };

}
