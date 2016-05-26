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
        auto terms  = seperate(sentence, language.seperators);
        auto tokens = Tokens();

        while (terms.size() > 0)
        {
            auto result = language.identify(terms);
            tokens.push_back(std::get<0>(result));
            terms = std::get<1>(result);
        }

        return tokens;
    }
}
