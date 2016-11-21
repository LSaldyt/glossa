#include "constructor.hpp"

namespace gen
{
Constructor::Constructor()
{
}
Constructor::Constructor(SymbolStorageGenerator set_symbol_storage_generator, Branch set_main_branch) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    main_branch(set_main_branch)
{
}

vector<string> evaluateBranch(Branch branch, SymbolStorage& symbol_storage)
{
    vector<string> generated;

    if (branch.condition_evaluator(symbol_storage))
    {
        for (auto line_constructor : branch.line_constructors)
        {
            generated.push_back(line_constructor(symbol_storage));
        }
        for (auto nested_branch : branch.nested_branches)
        {
            concat(generated, evaluateBranch(nested_branch, symbol_storage));
        }
    }
    return generated;
}

vector<string> Constructor::operator()(vector<vector<shared_ptr<Symbol>>>& symbol_groups)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    return evaluateBranch(main_branch, symbol_storage);
}
}
