#pragma once
#include "branch.hpp"
#include <type_traits>

namespace gen 
{

void addNewLine(vector<string>& generated);
void addNewLine(vector<shared_ptr<Symbol>>& generated);

/**
 * Constructs source code for a single syntax element
 */
template <typename T>
class Constructor
{
    Branch<T> main_branch;
    vector<string> definitions;

public:
    Constructor(){}

    /**
     * Builds a constructor
     * @param set_symbol_storage_generator Function for generating a symbol storage from token groups
     * @param set_main_branch The main branch of the constructor
     * @param set_definitions Defined names
     */
    Constructor(Branch<T> set_main_branch, 
                vector<string> set_definitions) : 
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
    vector<T> evaluateBranch(Branch<T> branch, 
                             unordered_set<string>& names, 
                             MultiSymbolTable& ms_table,
                             string filetype, 
                             int nesting=0, 
                             OutputManager logger=OutputManager(0))
    {
        logger.log("Evaluating branch", 2);
        vector<T> generated;

        if (branch.condition_evaluator(names, ms_table))
        {
            for (auto it = branch.line_constructors.begin(); it != branch.line_constructors.end(); it++)
            {
                auto line_constructor = *it;
                generated.push_back(line_constructor(names, ms_table, filetype, definitions, nesting, logger));
                if (it + 1 != branch.line_constructors.end())
                {
                    addNewLine(generated);
                }
            }
            for (auto nested_branch : branch.nested_branches)
            {
                concat(generated, evaluateBranch(nested_branch, names, ms_table, filetype, nesting, logger));
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
    vector<T> operator()(unordered_set<string>& names, 
                         MultiSymbolTable&      ms_table, 
                         string filetype, 
                         int nesting=0, 
                         OutputManager logger=OutputManager(0))
    {
        logger.log("Running constructor for " + filetype + " file", 2);
        auto generated = evaluateBranch(main_branch, names, ms_table, filetype, nesting);
        return generated;
    }
};
}

