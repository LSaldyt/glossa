#pragma once

// Avoid a circular include
namespace syntax
{
class Symbol;
}

#include "../tools/tools.hpp"
#include "../lex/seperate.hpp"
#include "../lex/language.hpp"
#include "../parse/tokenparsers.hpp"


namespace gen 
{

using namespace syntax;
using namespace parse;
using namespace lex;
using namespace match;
using namespace tools;

using SymbolTable      = unordered_map<string, shared_ptr<Symbol>>;
using MultiSymbolTable = unordered_map<string, vector<shared_ptr<Symbol>>>;
using SymbolStorage    = tuple<SymbolTable, MultiSymbolTable>;
using SymbolStorageGenerator = function<SymbolStorage(vector<vector<shared_ptr<Symbol>>>&)>;

using ConditionEvaluator = function<bool(unordered_set<string>&, SymbolStorage&, const vector<string>&)>;
using LineConstructor    = function<string(unordered_set<string>&, SymbolStorage&, bool)>;

const auto defaultBranch = [](unordered_set<string>&, SymbolStorage&, const vector<string>& generated){return true;};
const auto inverseBranch = [](ConditionEvaluator c)
{
    return [c](unordered_set<string>& names, SymbolStorage& symbol_storage, const vector<string>& generated)
    {
        auto condition = c(names, symbol_storage, generated);
        return not condition;
    };
};

struct Branch
{
    ConditionEvaluator condition_evaluator;
    vector<LineConstructor> line_constructors;
    vector<Branch> nested_branches;

    Branch(ConditionEvaluator set_condition_evaluator, vector<LineConstructor> set_line_constructors, vector<Branch> set_nested_branches);
    Branch();
};

}
