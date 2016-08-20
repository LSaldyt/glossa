#pragma once
#include "Symbols.hpp"
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
        std::string identifier;
        Expression value;

        std::string representation();
    };

    struct Function : public Statement
    {
        Function();

        std::string                             identifier;
        std::vector<std::string>                argnames;
        std::vector<std::shared_ptr<Statement>> body;
        Expression                              return_expression;

        std::string representation();
    };

    struct FunctionCall : public Statement
    {
        std::string              identifier;
        std::vector<std::string> args;
        std::string              representation();
    };
}
