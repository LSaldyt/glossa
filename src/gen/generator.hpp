#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"

namespace gen 
{

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

class Generator 
{

public:

    Generator(vector<string> grammar_files, string directory);
    vector<tuple<string, vector<string>>> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string symbol_type);

private:

    vector<tuple<string, FileConstructor>> file_constructors;

    tuple<Constructor, Constructor> read(string filename);
    void readStructureFile(string filename);

    unordered_map<string, tuple<Constructor, Constructor>> construction_map;
    Constructor generateConstructor(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    Branch generateBranch(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    LineConstructor generateLineConstructor(vector<string> terms);
    
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator generateConditionEvaluator(vector<string> terms);

};

}
