#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"

namespace gen 
{

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

/**
 * Class for generating source code from AST in a particular language
 */
class Generator 
{

public:

    Generator(vector<string> grammar_files, string directory);
    vector<tuple<string, string, vector<string>>> operator()(unordered_set<string>& names, vector<vector<shared_ptr<Symbol>>>&, string symbol_type, string filename="none", int nesting=1);

    vector<tuple<string, FileConstructor>> file_constructors;
private:
    unordered_map<string, vector<tuple<string, Constructor>>> construction_map;

    vector<tuple<string, Constructor>> readConstructor(string filename);
    void readStructureFile(string filename);

    string formatSymbol (string s, unordered_set<string>& names, SymbolStorage& storage, string filetype, vector<string>& definitions);

    Constructor            generateConstructor(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    Branch                 generateBranch(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    LineConstructor        generateLineConstructor(string line);
    LineConstructor        generateSpecialLineConstructor(string line);
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator     generateConditionEvaluator(vector<string> terms);

};

}
