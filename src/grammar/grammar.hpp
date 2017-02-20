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

using SymbolMatrix     = vector<vector<shared_ptr<Symbol>>>;
using IdentifiedGroups = vector<tuple<string, SymbolMatrix>>;
using StatementConstructor = function<shared_ptr<Symbol>(SymbolMatrix)>;
using GrammarMap = unordered_map<string, tuple<vector<SymbolicTokenParser>, vector<int>>>; 

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
    Grammar(vector<string> grammar_files, string directory, string lex_dir);

    IdentifiedGroups identifyGroups(vector<SymbolicToken>& tokens, OutputManager logger);

    vector<string> keywords;
    LexMap lexmap;

    vector<char> string_delimiters;

    string multiline_comment_delimiter;
    string comment_delimiter;
    vector<Seperator> whitespace; 

private:
    GrammarMap grammar_map; 

    void readSymbolFile(vector<string> symbol_file);

    tuple<string, vector<Result<SymbolicToken>>> identify (vector<SymbolicToken>& tokens, OutputManager logger);
    SymbolMatrix toGroup(string name, vector<Result<SymbolicToken>> results);

    tuple<vector<SymbolicTokenParser>, vector<int>> read(string filename);
    tuple<bool, vector<Result<SymbolicToken>>> evaluateGrammar(vector<SymbolicTokenParser> parsers, vector<SymbolicToken>& tokens, OutputManager logger);

    vector<SymbolicTokenParser> readGrammarPairs(vector<string>& terms);
    SymbolicTokenParser  readGrammarTerms(vector<string>& terms);
    SymbolicTokenParser  retrieveGrammar(string filename); 

    void readDelimiters(string directory);
    void readInherits(string directory);
    void readLexRules(string lex_dir);
};

}
