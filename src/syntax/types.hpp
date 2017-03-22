#pragma once
#include "../tools/tools.hpp"

namespace syntax
{
using namespace tools;

class Symbol;
using SymbolTable      = unordered_map<string, shared_ptr<Symbol>>;
using MultiSymbolTable = unordered_map<string, vector<shared_ptr<Symbol>>>;
using SymbolStorage    = tuple<SymbolTable, MultiSymbolTable>;
using SymbolStorageGenerator = function<SymbolStorage(vector<vector<shared_ptr<Symbol>>>&)>;

}
