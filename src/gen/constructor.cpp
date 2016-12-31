#include "constructor.hpp"
#include "../syntax/symbols/symbol.hpp"

namespace gen
{
Constructor::Constructor()
{
}
Constructor::Constructor(SymbolStorageGenerator set_symbol_storage_generator, 
                         Branch set_main_branch, 
                         vector<string> set_definitions) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    main_branch(set_main_branch),
    definitions(set_definitions)
{
}

vector<string> Constructor::evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage, string filetype)
{
    vector<string> generated;

    if (branch.condition_evaluator(names, symbol_storage, generated))
    {
        for (auto it = branch.line_constructors.begin(); it != branch.line_constructors.end(); it++)
        {
            auto line_constructor = *it;
            generated.push_back(line_constructor(names, symbol_storage, filetype, definitions));
            if (it + 1 != branch.line_constructors.end())
            {
                generated.push_back("\n");
            }
        }
        for (auto nested_branch : branch.nested_branches)
        {
            concat(generated, evaluateBranch(nested_branch, names, symbol_storage, filetype));
        }  
    }

    return generated;
}

vector<string> Constructor::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups, string filetype)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    auto generated = evaluateBranch(main_branch, names, symbol_storage, filetype);
    return generated;
}
}

