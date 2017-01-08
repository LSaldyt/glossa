#pragma once
#include "../lex/seperate.hpp"
#include "../lex/lexmap.hpp"
#include "../syntax/syntax.hpp"
#include "../parse/tokenparsers.hpp"
#include "../tools/tools.hpp"

namespace grammar
{

using namespace parse;
using namespace lex;
using namespace match;
using namespace syntax;
using namespace tools;

using StatementConstructor = function<shared_ptr<Symbol>(vector<vector<shared_ptr<Symbol>>>)>;
using GrammarMap = unordered_map<string, tuple<vector<SymbolicTokenParser>, vector<int>>>; 

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation);

template <typename T> 
shared_ptr<Symbol> createSymbol(T t, string annotation)
{
    return annotateSymbol(make_shared<T>(t), annotation);
}

class Grammar
{
public:
    Grammar(vector<string> grammar_files, string directory);

    vector<tuple<string, vector<vector<shared_ptr<Symbol>>>>> identifyGroups(SymbolicTokens& tokens);

    vector<string> keywords;
    LexMap lexmap;

    vector<char> string_delimiters;

    string multiline_comment_delimiter;
    string comment_delimiter;

private:
    GrammarMap grammar_map; 

    void readSymbolFile(vector<string> symbol_file);

    tuple<string, vector<Result<SymbolicToken>>> identify (SymbolicTokens& tokens);
    vector<vector<shared_ptr<Symbol>>> toGroup(string name, vector<Result<SymbolicToken>> results);

    tuple<vector<SymbolicTokenParser>, vector<int>> read(string filename);
    tuple<bool, vector<Result<SymbolicToken>>> evaluateGrammar(vector<SymbolicTokenParser> parsers, SymbolicTokens& tokens);

    vector<SymbolicTokenParser> readGrammarPairs(vector<string>& terms);
    SymbolicTokenParser  readGrammarTerms(vector<string>& terms);
    SymbolicTokenParser  retrieveGrammar(string filename); 

    void readDelimiters(string directory);
};

}
