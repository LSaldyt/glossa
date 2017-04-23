#pragma once 
#include "../tools/tools.hpp"
#include "../lex/seperate.hpp"
#include "../lex/lexmap.hpp"
#include "../parse/tokenparsers.hpp"
#include "../syntax/types.hpp"

namespace gen 
{

using namespace syntax;
using namespace parse;
using namespace lex;
using namespace match;
using namespace tools;

using ConditionEvaluator = function<bool(unordered_set<string>&, MultiSymbolTable&)>;
template <typename T>
using ElementConstructor = function<T(unordered_set<string>&, MultiSymbolTable&, string, vector<string>& definitions, int nesting, OutputManager logger)>;

template <typename T>
using ElementConstructorCreator = function<ElementConstructor<T>(string)>;

const auto defaultBranch = [](unordered_set<string>&, MultiSymbolTable&){return true;};
const auto inverseBranch = [](ConditionEvaluator c)
{
    return [c](unordered_set<string>& names, MultiSymbolTable& ms_table)
    {
        auto condition = c(names, ms_table);
        return not condition;
    };
};

/**
 * Optionally constructs pieces of source code if certain conditions are met
 * Allows user-defined code construction by enforcing meta-rules
 */
template <typename T>
struct Branch
{
    ConditionEvaluator condition_evaluator;
    vector<ElementConstructor<T>> line_constructors;
    vector<Branch<T>> nested_branches;

    Branch(ConditionEvaluator set_condition_evaluator, 
           vector<ElementConstructor<T>> set_line_constructors, 
           vector<Branch<T>> set_nested_branches)
    : condition_evaluator(set_condition_evaluator),
      line_constructors(set_line_constructors),
      nested_branches(set_nested_branches)
    {

    }
    Branch(){}
};

}
