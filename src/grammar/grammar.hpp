#pragma once
#include "../lex/seperate.hpp"
#include "../lex/lexmap.hpp"
#include "../syntax/syntax.hpp"
#include "../parse/tokenparsers.hpp"
#include "../tools/tools.hpp"

/**
 * Module that defines meta-rules for user-defined parsing of programming languages
 */
namespace grammar
{

using namespace parse;
using namespace lex;
using namespace match;
using namespace syntax;
using namespace tools;

//using SymbolMatrix     = vector<vector<shared_ptr<Symbol>>>;
//using IdentifiedGroups = vector<tuple<string, SymbolMatrix>>;
//using MultiSymbolTable = unordered_map<string, vector<shared_ptr<Symbol>>>;
using IdentifiedGroups = vector<tuple<string, MultiSymbolTable>>;
//using StatementConstructor = function<shared_ptr<Symbol>(MultiSymbolTable)>;
using GrammarMap = unordered_map<string, tuple<vector<SymbolicTokenParser>, vector<tuple<int, string>>>>; 

vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken>);

shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation);

template <typename T> 
shared_ptr<Symbol> createSymbol(T t, string annotation)
{
    return annotateSymbol(make_shared<T>(t), annotation);
}

/**
 * Class that allows parsing of a particular programming language
 * Constructed from a set of grammar files
 */
class Grammar
{
public:
    Grammar(string grammarfile, string lex_dir);

    IdentifiedGroups identifyGroups(vector<SymbolicToken>& tokens, OutputManager logger);

    vector<string> keywords;

private:

    void readGrammarFile(string filename);

    GrammarMap grammar_map; 

    void readSymbolFile(vector<string> symbol_file);

    tuple<string, vector<Result<SymbolicToken>>> identify (vector<SymbolicToken>& tokens, OutputManager logger);
    MultiSymbolTable createMultiSymbolTable(string name, vector<Result<SymbolicToken>> results);

    tuple<bool, vector<Result<SymbolicToken>>> evaluateGrammar(vector<SymbolicTokenParser> parsers, vector<SymbolicToken>& tokens, OutputManager logger);

    vector<SymbolicTokenParser> readGrammarPairs(vector<string>& terms);
    SymbolicTokenParser  readGrammarTerms(vector<string>& terms);
    SymbolicTokenParser  retrieveGrammar(string filename); 

    void readInherits(string directory);

};

}
