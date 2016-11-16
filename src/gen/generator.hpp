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

using Constructor = function<vector<string>(vector<vector<shared_ptr<Symbol>>>)>;

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);


class Generator 
{
    const static unordered_map<string, Constructor> construction_map;

public:
    Generator(vector<string> grammar_files, string directory);
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
