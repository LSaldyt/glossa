#pragma once
#include "statement.hpp"

namespace syntax
{
    struct SuperCall : public Statement
    {
        SuperCall(vector<vector<shared_ptr<Symbol>>> symbol_groups);

        vector<shared_ptr<Symbol>> args;

        string source(unordered_set<string>&, string n_space="");
    };
}
