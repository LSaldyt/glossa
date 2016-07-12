#include "Lex.hpp"

namespace Lex
{
    Tokens lex(const std::string& sentence, const Language& language)
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
