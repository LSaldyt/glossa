#pragma once
#include "branch.hpp"

namespace gen 
{

/**
 * Constructs source code for a single syntax element
 */
class Constructor
{
    SymbolStorageGenerator symbol_storage_generator;
    Branch main_branch;
    vector<string> definitions;

public:
    Constructor();
    Constructor(SymbolStorageGenerator set_symbol_storage_generator, 
                Branch set_main_branch, 
                vector<string> set_definitions);

    vector<string> evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage, string filetype, int nesting=0, OutputManager logger=OutputManager(0));
    vector<string> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string filetype, int nesting=0, OutputManager logger=OutputManager(0));
};
}

