#include "../Syntax/Symbols.hpp"
#include "../Parser/Parser.hpp"
#include "../Parser/ParserBase/Locale.hpp"
#include "LexResult.hpp"

class Lexer
{
    Syntax::SymbolGenerators symbols;
    Parse::Parser<Parse::ParseFunction>   parser;

public:
    Lexer (Syntax::SymbolGenerators set_symbols=Syntax::SymbolGenerators(), Parse::Parser<Parse::ParseFunction> set_parser = Parse::Parser<Parse::ParseFunction>());
    ~Lexer();

    LexResult lex(const std::string& sentence);
    LexResult lex(Parse::Terms terms);

private:
    LexResult match(Parse::Result parse_result);

};
