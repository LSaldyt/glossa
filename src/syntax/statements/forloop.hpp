#pragma once
#include "statement.hpp"

namespace syntax
{

struct ForLoop : public Statement
{
    ForLoop(vector<vector<shared_ptr<Symbol>>> symbol_groups);
    string                     loop_variable;
    shared_ptr<Symbol>         loop_source;
    vector<shared_ptr<Symbol>> body;
    
    string source(unordered_set<string>&);
};

}
