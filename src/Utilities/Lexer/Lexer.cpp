#include "Lexer.hpp"

namespace Lexer
{
    Lexer::Lexer(const Language& set_language)
    {
        language = set_language;
    }

    Lexer::~Lexer(){}

    LexResult Lexer::lex(const std::string& sentence)
    {
        auto terms = seperate(sentence, language.seperators);
        return LexResult();
    }
}
