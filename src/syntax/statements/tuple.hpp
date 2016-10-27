#pragma once
#include "statement.hpp"

namespace syntax
{

struct Tuple : public Symbol
{
    vector<shared_ptr<Symbol>> content;
    Tuple(vector<vector<shared_ptr<Symbol>>> symbol_groups);

    string source(unordered_set<string>& names, string n_space="");
};

}
