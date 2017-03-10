#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"
#include "read.hpp"

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
    vector<tuple<string, string, vector<string>>> operator()
        (unordered_set<string>& names, 
         MultiSymbolTable&, 
         string symbol_type, 
         string filename="none", 
         int nesting=1, 
         OutputManager logger=OutputManager(1));

    vector<tuple<string, FileConstructor>> file_constructors;
private:
    unordered_map<string, vector<tuple<string, Constructor<string>>>> construction_map;

    string formatSymbol (string s, unordered_set<string>& names, MultiSymbolTable& ms_table, string filetype, vector<string>& definitions);

    vector<tuple<string, Constructor<string>>> readConstructor(string filename);
    void readStructureFile(string filename);

    ElementConstructor<string> generateElementConstructor(string line);
    ElementConstructor<string> generateSpecialElementConstructor(string line);

    ElementConstructorCreator<string> ec_creator;
    Constructor<string> default_constructor;
};

}
