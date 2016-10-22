#pragma once
#include "statement.hpp"

namespace syntax
{

struct Vector : public Symbol
{
    vector<shared_ptr<Symbol>> content;
    Vector(vector<vector<shared_ptr<Symbol>>> symbol_groups);

    string source(unordered_set<string>& names);
};

}
