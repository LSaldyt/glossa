#pragma once
#include "statement.hpp"

namespace syntax
{
    struct Function : public Statement
    {
        Function();
        Function(vector<vector<shared_ptr<Symbol>>>);

        string                     identifier;
        vector<string>             argnames;
        vector<shared_ptr<Symbol>> body;
        shared_ptr<Symbol>         return_expression;

        string source(unordered_set<string>&, string n_space="");
        string header(unordered_set<string>&, string n_space="");
        string name();
    };
}
