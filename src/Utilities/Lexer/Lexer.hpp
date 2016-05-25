#include "../Syntax/Symbols.hpp"
#include "../Syntax/Token.hpp"
#include "../Parser/Parse.hpp"
#include "Language.hpp"

namespace Lexer
{
    using namespace Syntax;

    class Lexer
    {
        Language language;

    public:
        Lexer (const Language& set_language);
        ~Lexer();

        Tokens lex(const std::string& sentence);
    };
}
