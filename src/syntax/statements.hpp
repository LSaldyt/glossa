#pragma once
#include "import.hpp"
#include "symbols.hpp"
#include "../types/symbolictoken.hpp"

namespace Syntax
{
    struct Statement : public Symbol
    {
        virtual string representation();
    };

    struct Expression : public Statement
    {
        Expression();

        shared_ptr<Symbol> base;
        vector<tuple<shared_ptr<Symbol>, shared_ptr<Symbol>>> extensions;

        string representation();
    };

    struct Assignment : public Statement
    {
        Assignment(vector<shared_ptr<Symbol>>);
        Assignment();

        string identifier;
        shared_ptr<Symbol> value;

        string representation();
    };

    struct Function : public Statement
    {
        Function();

        string                             identifier;
        vector<string>                argnames;
        vector<shared_ptr<Statement>> body;
        shared_ptr<Symbol>                 return_expression;

        string representation();
    };

    struct FunctionCall : public Statement
    {
        FunctionCall();
        FunctionCall(vector<shared_ptr<Symbol>>);

        string              identifier;
        vector<shared_ptr<Symbol>> args;

        string              representation();
    };

    struct Conditional : public Statement
    {
        Expression condition;

        vector<shared_ptr<Statement>> a; // If
        vector<shared_ptr<Statement>> b; // Else
    };
}
