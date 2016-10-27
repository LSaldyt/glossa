#pragma once
#include "statement.hpp"

namespace syntax 
{

struct Import : public Symbol
{
    string filename;

    Import(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string source(unordered_set<string>& names, string n_space="");
    string header(unordered_set<string>& names, string n_space="");
};

}
