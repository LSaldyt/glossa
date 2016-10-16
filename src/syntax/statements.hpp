#pragma once
#include "import.hpp"
#include "symbols.hpp"
#include "../types/symbolictoken.hpp"

namespace syntax
{
    struct Statement : public Symbol
    {
        virtual string representation();
    };

    struct Expression : public Statement
    {
        Expression();
        Expression(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        shared_ptr<Symbol> base;
        vector<tuple<shared_ptr<Symbol>, shared_ptr<Symbol>>> extensions;

        string representation();
        string source(unordered_set<string>&);
    };

    struct Assignment : public Statement
    {
        Assignment(vector<vector<shared_ptr<Symbol>>>);

        string identifier;
        shared_ptr<Symbol> value;

        string representation();
        string source(unordered_set<string>&);
        string name();
    };

    struct Function : public Statement
    {
        Function();
        Function(vector<vector<shared_ptr<Symbol>>>);

        string                     identifier;
        vector<string>             argnames;
        vector<shared_ptr<Symbol>> body;
        shared_ptr<Symbol>         return_expression;

        string representation();
        string source(unordered_set<string>&);
        string name();
    };

    struct FunctionCall : public Statement
    {
        FunctionCall();
        FunctionCall(vector<vector<shared_ptr<Symbol>>>);

        string              identifier;
        vector<shared_ptr<Symbol>> args;

        string              representation();
        string source(unordered_set<string>&);
    };

    struct Conditional : public Statement
    {
        Conditional(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        shared_ptr<Symbol> condition;
        vector<shared_ptr<Symbol>> if_body;
        vector<shared_ptr<Symbol>> else_body;

        string representation();
        string source(unordered_set<string>&);
    };
}
