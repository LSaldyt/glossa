#pragma once
#include "statement.hpp"

namespace syntax
{

struct WhileLoop : public Statement
{
    WhileLoop(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    shared_ptr<Symbol>         condition;
    vector<shared_ptr<Symbol>> body;
    
    string source(unordered_set<string>&, string n_space="");
};

}
