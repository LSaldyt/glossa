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
#include "transform/transformer.hpp"

namespace compiler
{
    using namespace gen;
    using namespace lex;
    using namespace tools;
    using namespace parse;
    using namespace syntax;
    using namespace grammar;
    using namespace transform;

    void compileFiles(vector<string> filenames, string input_dir, string input_lang, string output_dir, string output_lang, int verbosity=1);
    void compile(string filename, Grammar& grammar, Generator& generator, TransformerMap& transformer, 
                 unordered_map<string, string>& symbol_table, 
                 string input_directory="", string output_directory="", 
                 OutputManager logger=OutputManager(1));

    Grammar   loadGrammar   (string language);
    Generator loadGenerator (string language);
    TransformerMap loadTransformer (string language);

    unordered_map<string, string> readSymbolTable(string filename);

    vector<Tokens>                tokenPass(vector<string>, Grammar&, unordered_map<string, string>&, OutputManager logger);
    vector<vector<SymbolicToken>> symbolicPass(vector<Tokens> tokens, OutputManager logger);
    vector<SymbolicToken>         join(vector<vector<SymbolicToken>>, bool newline=false);

    unordered_map<string, tuple<vector<string>, string>> compileGroups(vector<tuple<string, SymbolMatrix>> identified_groups,
                                                                       string filename,
                                                                       Generator& generator,
                                                                       OutputManager logger);
    void showAST(const IdentifiedGroups& identified_groups, OutputManager logger);
}
