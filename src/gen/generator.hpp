#pragma once
#include "constructor.hpp"

namespace gen 
{

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);


class Generator 
{
    const static unordered_map<string, Constructor> construction_map;

public:
    Generator(vector<string> grammar_files, string directory);
    tuple<Constructor, Constructor> read(string filename);
    Constructor generateConstructor(vector<string> content, SymbolStorageGenerator symbol_storage_generator);
    
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator generateConditionEvaluator(vector<string> terms);

    /*
    vector<shared_ptr<Symbol>> constructFrom(SymbolicTokens& tokens);

private:
    
    shared_ptr<Symbol> build(string name, vector<vector<shared_ptr<Symbol>>> symbols);

    tuple<string, vector<Result<SymbolicToken>>> identify (SymbolicTokens& tokens);
    shared_ptr<Symbol> construct(string name, vector<Result<SymbolicToken>> results);

    tuple<SymbolicTokenParsers, vector<int>> read(string filename);
    tuple<bool, vector<Result<SymbolicToken>>> evaluateGrammar(SymbolicTokenParsers parsers, SymbolicTokens& tokens);

    SymbolicTokenParsers readGrammarPairs(vector<string>& terms);
    SymbolicTokenParser  readGrammarTerms(vector<string>& terms);
    SymbolicTokenParser  retrieveGrammar(string filename); 
    */

};

}
