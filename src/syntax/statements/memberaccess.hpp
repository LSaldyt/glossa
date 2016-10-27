#pragma once
#include "statement.hpp"

namespace syntax
{
    struct MemberAccess : public Statement
    {
        MemberAccess(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        string identifier;
        shared_ptr<Symbol> value;

        string source(unordered_set<string>&, string n_space="");
    };
}
