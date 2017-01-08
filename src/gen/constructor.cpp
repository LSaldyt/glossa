#include "constructor.hpp"
#include "../syntax/symbols/symbol.hpp"

namespace gen
{
Constructor::Constructor()
{
}

/**
 * Builds a constructor
 * @param set_symbol_storage_generator Function for generating a symbol storage from token groups
 * @param set_main_branch The main branch of the constructor
 * @param set_definitions Defined names
 */
Constructor::Constructor(SymbolStorageGenerator set_symbol_storage_generator, 
                         Branch set_main_branch, 
                         vector<string> set_definitions) : 
    symbol_storage_generator(set_symbol_storage_generator), 
    main_branch(set_main_branch),
    definitions(set_definitions)
{
}

/**
 * Evaluates a branch within a constructor
 * @param names Namespace
 * @param symbol_storage Dictionary of stored symbols, user defined
 * @param filetype Target filetype (i.e. header or source if in c-like languages)
 * @param nesting Level of indentation
 * @return Lines of source code
 */
vector<string> Constructor::evaluateBranch(Branch branch, unordered_set<string>& names, SymbolStorage& symbol_storage, string filetype, int nesting)
{
    vector<string> generated;

    if (branch.condition_evaluator(names, symbol_storage, generated))
    {
        for (auto it = branch.line_constructors.begin(); it != branch.line_constructors.end(); it++)
        {
            auto line_constructor = *it;
            generated.push_back(line_constructor(names, symbol_storage, filetype, definitions, nesting));
            if (it + 1 != branch.line_constructors.end())
            {
                generated.push_back("\n");
            }
        }
        for (auto nested_branch : branch.nested_branches)
        {
            concat(generated, evaluateBranch(nested_branch, names, symbol_storage, filetype, nesting));
        }  
    }

    return generated;
}

/**
 * Constructs a syntax element from symbol_groups
 * @param names Namespace
 * @param symbol_groups 2D array of shared_ptr<Symbol>s
 * @param filetype String description of filetype
 * @param nesting Indentation level
 * @return Lines of source code representing the given syntax element
 */
vector<string> Constructor::operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>& symbol_groups, string filetype, int nesting)
{
    auto symbol_storage = symbol_storage_generator(symbol_groups);
    auto generated = evaluateBranch(main_branch, names, symbol_storage, filetype, nesting);
    return generated;
}
}

