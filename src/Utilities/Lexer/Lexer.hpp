#include "../Syntax/Symbols.hpp"
#include "../Parser/Parse.hpp"
#include "../Parser/ParserBase/Locale.hpp"
#include "Language.hpp"
#include "LexResult.hpp"

namespace Lexer
{
    class Lexer
    {
        Language language;

    public:
        Lexer (const Language& set_language);
        ~Lexer();

        LexResult lex(const std::string& sentence);

    private:
        LexResult match(Parse::Result parse_result);

    };
}
