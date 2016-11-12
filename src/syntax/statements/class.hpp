#pragma once
#include "statement.hpp"

namespace syntax
{
struct Class : public Symbol
{
    string name;
    string inheritance;
    shared_ptr<Symbol> constructor;
    vector<shared_ptr<Symbol>> body;

    Class(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string source(unordered_set<string>& names, string n_space="");
    string header(unordered_set<string>& names, string n_space="");
};
}
