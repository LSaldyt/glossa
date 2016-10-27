#pragma once
#include "statement.hpp"

namespace syntax
{
struct Class : public Symbol
{
    string name;
    string inheritance;
    vector<shared_ptr<Symbol>> body;

    Class(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string source(unordered_set<string>& names);
    string header(unordered_set<string>& names);
};
}
