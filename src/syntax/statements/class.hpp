#pragma once
#include "statement.hpp"

namespace syntax
{
struct Class : public Symbol
{
    string name;
    string inheritance;
    shared_ptr<Symbol> super_call;
    vector<string>             members;
    vector<shared_ptr<Symbol>> constructor_args;
    vector<shared_ptr<Symbol>> constructor_body;
    vector<shared_ptr<Symbol>> body;

    Class(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string source(unordered_set<string>& names, string n_space="");
    string header(unordered_set<string>& names, string n_space="");
};
}
