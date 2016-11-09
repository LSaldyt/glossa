#pragma once
#include "statement.hpp"

namespace syntax
{
    struct Constructor : public Statement
    {
        Constructor();
        Constructor(vector<vector<shared_ptr<Symbol>>>);

        vector<string>             argnames;
        vector<shared_ptr<Symbol>> body;
        shared_ptr<Symbol>         return_expression;

        string source(unordered_set<string>&, string n_space="");
        string header(unordered_set<string>&, string n_space="");
    };
}
