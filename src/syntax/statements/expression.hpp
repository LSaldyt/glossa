#pragma once
#include "statement.hpp"

namespace syntax
{
    struct Expression : public Statement
    {
        Expression();
        Expression(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        shared_ptr<Symbol> base;
        vector<tuple<shared_ptr<Symbol>, shared_ptr<Symbol>>> extensions;

        string representation();
        string source(unordered_set<string>&);
    };
}
