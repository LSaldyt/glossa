#include "../Syntax/Symbols.hpp"
#include "../Parser/Parse.hpp"
#include "../Parser/ParserBase/Locale.hpp"
#include "LexResult.hpp"

namespace Lexer
{
    class Lexer
    {
        SymbolGenerators symbols;
        ParseFunction    parser;
        Seperators       seperators;

    public:
        Lexer (const SymbolGenerators& set_symbols    = SymbolGenerators(),
               const ParseFunction     set_parser     = wildcard,
               const Seperators&       set_seperators = Seperators());
        ~Lexer();

        LexResult lex(const std::string& sentence);

    private:
        LexResult match(Parse::Result parse_result);

    };
}
