#pragma once
#include "tools/tools.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"
#include "lex/seperate.hpp"
#include "lex/lexmap.hpp"
#include "types/symbolize.hpp"
#include "grammar/grammar.hpp"
#include "gen/gen.hpp"
#include "gen/generator.hpp"

namespace compiler
{
    using namespace parse;
    using namespace lex;
    using namespace syntax;
    using namespace grammar;
    using namespace gen;
    using namespace tools;

    void compileFiles(vector<string> filenames, string input_dir, string input_lang, string output_dir, string output_lang, int verbosity=1);
    void compile(string filename, Grammar& grammar, Generator& generator, unordered_map<string, string>& symbol_table, string input_directory="", string output_directory="", OutputManager logger=OutputManager(1));
    Grammar loadGrammar(string language);
    Generator loadGenerator(string language);

    std::vector<Tokens> tokenPass(std::vector<std::string>, Grammar&, unordered_map<string, string>&, OutputManager logger);
    std::vector<vector<SymbolicToken>> symbolicPass(std::vector<Tokens> tokens, OutputManager logger);
    vector<SymbolicToken> join(std::vector<vector<SymbolicToken>>, bool newline=false);
    unordered_map<string, string> readSymbolTable(string filename);
}
