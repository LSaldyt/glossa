#pragma once
#include "branch.hpp"

namespace gen 
{

class Constructor
{
    SymbolStorageGenerator symbol_storage_generator;
    Branch main_branch;

public:
    Constructor();
    Constructor(SymbolStorageGenerator set_symbol_storage_generator, Branch set_main_branch);
    vector<string> operator()(vector<vector<shared_ptr<Symbol>>>&);
};
}

