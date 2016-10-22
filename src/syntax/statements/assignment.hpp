#pragma once
#include "statement.hpp"

namespace syntax
{
    struct Assignment : public Statement
    {
        Assignment(vector<vector<shared_ptr<Symbol>>>);

        string identifier;
        shared_ptr<Symbol> value;

        string representation();
        string source(unordered_set<string>&);
        string name();
    };
}
