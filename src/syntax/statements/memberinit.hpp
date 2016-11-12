#pragma once
#include "statement.hpp"

namespace syntax
{
    struct MemberInit : public Statement
    {
        MemberInit(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        string identifier;
        shared_ptr<Symbol> value;

        string source(unordered_set<string>&, string n_space="");
        string name();
    };
}
