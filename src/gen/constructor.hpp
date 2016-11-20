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

using ConditionEvaluator = function<bool(SymbolStorage&)>;
using LineConstructor    = function<string(SymbolStorage&)>;

using Branch = tuple<ConditionEvaluator, vector<LineConstructor>>;
using BranchPoint = tuple<Branch, vector<Branch>>; 

const auto defaultBranch = [](SymbolStorage&){return true;};

class Constructor
{
    SymbolStorageGenerator symbol_storage_generator;
    vector<BranchPoint>    branches;

public:
    Constructor();
    Constructor(auto set_symbol_storage_generator, auto set_branches);
    vector<string> operator()(vector<vector<shared_ptr<Symbol>>>&);
};
}

