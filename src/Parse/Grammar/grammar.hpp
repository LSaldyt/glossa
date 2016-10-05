#pragma once
#include <tuple>
#include <vector>
#include <string>
#include <unordered_map>

#include "../../Lex/Seperate.hpp"
#include "../../Syntax/Statements.hpp"
#include "../TokenParsers.hpp"
#include "../../tools/tools.hpp"


namespace Grammar
{

using namespace Parse;
using namespace Match;
using namespace Syntax;
using namespace tools;

using StatementConstructor = function<shared_ptr<Symbol>(vector<shared_ptr<Symbol>>)>;

using GrammarMap = unordered_map<string, tuple<SymbolicTokenParsers, vector<int>>>; 

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

class Grammar
{
    const static unordered_map<string, StatementConstructor> construction_map;

public:
    Grammar(vector<string> grammar_files, string directory);

    vector<shared_ptr<Symbol>> constructFrom(SymbolicTokens& tokens);

    vector<string> keywords;

private:
    GrammarMap grammar_map; 

    shared_ptr<Symbol> build(string name, vector<shared_ptr<Symbol>> symbols);

    tuple<string, vector<Result<SymbolicToken>>> identify (SymbolicTokens& tokens);
    shared_ptr<Symbol> construct(string name, vector<Result<SymbolicToken>> results);

    tuple<SymbolicTokenParsers, vector<int>> read(string filename);
    tuple<bool, vector<Result<SymbolicToken>>> evaluateGrammar(SymbolicTokenParsers parsers, SymbolicTokens& tokens);

    SymbolicTokenParsers readGrammarPairs(vector<string>& terms);
    SymbolicTokenParser  readGrammarTerms(vector<string>& terms);
    SymbolicTokenParser  retrieveGrammar(string filename); 

};

}
