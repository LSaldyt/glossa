#include "constructor.hpp"
#include "../syntax/symbols/symbol.hpp"

namespace gen
{
Constructor::Constructor()
{
}
Constructor::Constructor(SymbolStorageGenerator set_symbol_storage_generator, Branch set_main_branch, vector<string> set_definitions, vector<string> set_name_indices) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    main_branch(set_main_branch),
    definitions(set_definitions),
    name_indices(set_name_indices)
{
}

vector<string> Constructor::evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage, string filetype)
{
    vector<string> generated;

    for (auto definition : definitions)
    {
        auto new_name = get<0>(symbol_storage)[definition]->name();
        print("Defined: " + new_name);
        names.insert(new_name);
    }
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
    return generated;
}

vector<string> Constructor::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups, string filetype)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    return evaluateBranch(main_branch, names, symbol_storage, filetype);
}

string Constructor::name()
{
    /*
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    if (name.size() == 2)
    {
        int index_a = std::stoi(name[0]);
        int index_b = std::stoi(name[1]);
        return symbol_storage[index_a][index_b]->name();
    }
    else
    {
        return "none";
    }
    */
    return "none";
}

}

