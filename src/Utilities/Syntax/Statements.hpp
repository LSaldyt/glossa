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
    using GeneratorFunction  = std::function<std::string(std::shared_ptr<Statement>)>;

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
            return (type + " " + identifier + " = " + "nah");
        }
    };

    const StatementGenerator AssignmentGenerator = [](SymbolicTokens tokens)
    {
        auto identifier = ""; //std::get<0>(tokens[0])->name;
        auto value      = std::get<0>(tokens[2]);
        auto type       = std::get<1>(tokens[2]);
        return std::make_shared<Assignment>(Assignment(identifier, value, type));
    };
}
