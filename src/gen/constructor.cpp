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
        for (auto line_constructor : branch.line_constructors)
        {
            generated.push_back(line_constructor(names, symbol_storage, filetype));
        }
        for (auto nested_branch : branch.nested_branches)
        {
            concat(generated, evaluateBranch(nested_branch, names, symbol_storage, filetype));
        }
    }

    for (auto definition : definitions)
    {
        assert(contains(get<0>(symbol_storage), definition));
        auto new_name = get<0>(symbol_storage)[definition]->name();
        if (not contains(names, new_name))
        {
            print("Namespace:");
            for (auto name : names)
            {
                print(name);
            }
            print("Adding name: " + new_name);
            names.insert(new_name);
        }
    }

    return generated;
}

vector<string> Constructor::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups, string filetype)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    return evaluateBranch(main_branch, names, symbol_storage, filetype);
}
}

