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
    vector<tuple<string, string, vector<string>>> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string symbol_type, string filename="none");

private:
    vector<tuple<string, FileConstructor>> file_constructors;
    unordered_map<string, vector<tuple<string, Constructor>>> construction_map;

    vector<tuple<string, Constructor>> readConstructor(string filename);
    void readStructureFile(string filename);

    Constructor            generateConstructor(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    Branch                 generateBranch(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    LineConstructor        generateLineConstructor(vector<string> terms);
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator     generateConditionEvaluator(vector<string> terms);

};

}
