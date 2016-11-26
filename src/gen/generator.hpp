#pragma once
#include "constructor.hpp"

namespace gen 
{

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

using File = vector<string>;


class Generator 
{

public:

    Generator(vector<string> grammar_files, string directory);
    vector<tuple<string, vector<string>>> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string symbol_type);

private:

    unordered_map<string, tuple<Constructor, Constructor>> construction_map;
    tuple<Constructor, Constructor> read(string filename);
    Constructor generateConstructor(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    Branch generateBranch(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    LineConstructor generateLineConstructor(vector<string> terms);
    
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator generateConditionEvaluator(vector<string> terms);

};

}
