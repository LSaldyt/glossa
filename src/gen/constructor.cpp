#include "constructor.hpp"

namespace gen
{
Constructor::Constructor()
{
}
Constructor::Constructor(auto set_symbol_storage_generator, auto set_branches) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    branches(set_branches)
{
}

// using Branch = tuple<ConditionEvaluator, vector<LineConstructor>>;
// using BranchPoint = tuple<Branch, vector<Branch>>; 

vector<string> evaluateBranch(BranchPoint branch_point, SymbolStorage& symbol_storage)
{
    vector<string> generated;
    auto branch              = get<0>(branch_point);
    auto nested_branches     = get<1>(branch_point);
    auto condition_evaluator = get<0>(branch);
    auto line_constructors   = get<1>(branch);

    if (condition_evaluator(symbol_storage))
    {
        for (auto line_constructor : line_constructors)
        {
            generated.push_back(line_constructor(symbol_storage));
        }
        for (auto nested_branch : nested_branches)
        {
            auto inner_condition_evaluator = get<0>(nested_branch);
            auto inner_line_constructors   = get<1>(nested_branch);

            if (inner_condition_evaluator(symbol_storage))
            {
                for (auto line_constructor : inner_line_constructors)
                {
                    generated.push_back(line_constructor(symbol_storage));
                }
            }
        }
    }
    return generated;
}

vector<string> Constructor::operator()(vector<vector<shared_ptr<Symbol>>>& symbol_groups)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);

    vector<string> generated;

    for (auto branch : branches)
    {
        concat(generated, evaluateBranch(branch, symbol_storage));
    }

    return generated;
}
}
