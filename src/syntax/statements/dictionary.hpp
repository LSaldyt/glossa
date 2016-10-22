#pragma once
#include "statement.hpp"

namespace syntax
{
struct Dictionary : public Symbol
{
    vector<shared_ptr<Symbol>> keys;
    vector<shared_ptr<Symbol>> values;

    Dictionary(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string source(unordered_set<string>& names);
};
}
