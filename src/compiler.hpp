#pragma once
#include "tools/tools.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"
#include "lex/seperate.hpp"
#include "lex/language.hpp"
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

    void compile(string filename, Grammar& grammar, Generator& generator, unordered_map<string, string>& symbol_table, string input_directory="", string output_directory="");
    Grammar loadGrammar(string language);
    Generator loadGenerator(string language);

    std::vector<Tokens> tokenPass(std::vector<std::string>, Grammar&, unordered_map<string, string>&);
    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens);
    SymbolicTokens join(std::vector<SymbolicTokens>);
    unordered_map<string, string> readSymbolTable(string filename);
}
