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

        // Convert a line to a list of tokens based on the language definition
        Tokens lex(const std::string& sentence);
    };
}
