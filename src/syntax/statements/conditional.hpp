#pragma once
#include "statement.hpp"

namespace syntax
{
    struct Conditional : public Statement
    {
        Conditional(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        shared_ptr<Symbol> condition;
        vector<shared_ptr<Symbol>> if_body;
        vector<shared_ptr<Symbol>> else_body;

        string source(unordered_set<string>&, string n_space="");
    };
}
