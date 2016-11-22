#include "constructor.hpp"
#include "../syntax/symbols/symbol.hpp"

namespace gen
{
Constructor::Constructor()
{
}
Constructor::Constructor(SymbolStorageGenerator set_symbol_storage_generator, Branch set_main_branch, vector<string> set_definitions) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    main_branch(set_main_branch),
    definitions(set_definitions)
{
}

vector<string> Constructor::evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage)
{
    vector<string> generated;

    print("Evaluating branch");

    for (auto definition : definitions)
    {
        auto new_name = get<0>(symbol_storage)[definition]->name();
        print("Defined: " + new_name);
        names.insert(new_name);
    }

    if (branch.condition_evaluator(names, symbol_storage))
    {
        for (auto line_constructor : branch.line_constructors)
        {
            generated.push_back(line_constructor(symbol_storage));
        }
        for (auto nested_branch : branch.nested_branches)
        {
            concat(generated, evaluateBranch(nested_branch, names, symbol_storage));
        }
    }
    else
    {
        print("Condition evaluation for branch failed");
    }
    return generated;
}

vector<string> Constructor::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    return evaluateBranch(main_branch, names, symbol_storage);
}
}
