#pragma once
#include "Symbols.hpp"
#include "../Types/SymbolicToken.hpp"

namespace Syntax
{
    struct Statement : public Symbol
    {
        virtual std::string representation();
    };

    struct Expression : public Statement
    {
        Expression();

        std::shared_ptr<Symbol> base;
        std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>> extensions;

        std::string representation();
    };

    struct Assignment : public Statement
    {
        Assignment(std::vector<std::shared_ptr<Symbol>>);
        Assignment();

        std::string identifier;
        std::shared_ptr<Symbol> value;

        std::string representation();
    };

    struct Function : public Statement
    {
        Function();

        std::string                             identifier;
        std::vector<std::string>                argnames;
        std::vector<std::shared_ptr<Statement>> body;
        std::shared_ptr<Symbol>                 return_expression;

        std::string representation();
    };

    struct FunctionCall : public Statement
    {
        std::string              identifier;
        std::vector<std::string> args;
        std::string              representation();
    };

    struct Conditional : public Statement
    {
        Expression condition;

        std::vector<std::shared_ptr<Statement>> a; // If
        std::vector<std::shared_ptr<Statement>> b; // Else
    };
}
