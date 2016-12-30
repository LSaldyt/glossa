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
        for (int i = 0; i < branch.line_constructors.size(); i++)
        {
            auto line_constructor = branch.line_constructors[i];
            generated.push_back(line_constructor(names, symbol_storage, filetype, definitions));
            if (i + 1 != branch.line_constructors.size())
            {
                generated.push_back("\n");
            }
        }
        for (int i = 0; i < branch.nested_branches.size(); i++)
        {
            auto nested_branch = branch.nested_branches[i];
            concat(generated, evaluateBranch(nested_branch, names, symbol_storage, filetype));
            if (i + 1 != branch.nested_branches.size())
            {
                generated.push_back("\n");
            }
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

