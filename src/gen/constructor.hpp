#pragma once
#include "branch.hpp"

namespace gen 
{

class Constructor
{
    SymbolStorageGenerator symbol_storage_generator;
    Branch main_branch;
    vector<string> definitions;
    vector<string> name_indices;

public:
    Constructor();
    Constructor(SymbolStorageGenerator set_symbol_storage_generator, Branch set_main_branch, vector<string> set_definitions, vector<string> set_name_indices);

    vector<string> evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage, string filetype);
    vector<string> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string filetype);
    string name();
};
}

