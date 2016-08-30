#pragma once
#include "Seperate.hpp"

namespace Lex
{
    using LanguageTermSet  = std::tuple<Terms, std::string>;
    using LanguageTermSets = std::vector<LanguageTermSet>;

    // A single definition for lexing a language element (ie int, operator)
    struct LanguageLexer
    {
        std::function<Result<std::string>(std::vector<std::string>)> match;
        std::string name;
        std::string type;
        int precedence;
        LanguageLexer(std::function<Result<std::string>(std::vector<std::string>)> set_match_function, 
                      std::string set_name, 
                      std::string set_type, 
                      int set_precedence);
    };

    using LanguageLexers  = std::vector<LanguageLexer>;

    // A collection of lexers/term sets that can Identify terms
    struct Language
    {
        Seperators    seperators;

        LanguageTermSets language_term_sets;
        LanguageLexers   language_lexers;

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageLexers&   set_language_lexers);

        std::tuple<Token, Terms> identify(Terms terms) const;
    };
}
