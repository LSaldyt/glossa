#include "Lexer.hpp"

namespace Lexer
{
    Lexer::Lexer(const Language& set_language)
    {
        language = set_language;
    }

    Lexer::~Lexer(){}

    Tokens Lexer::lex(const std::string& sentence)
    {
        auto terms = seperate(sentence, language.seperators);
        return Tokens();
    }
}
