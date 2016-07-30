#pragma once
#include "Seperate.hpp"

namespace Lex
{
    using LanguageTermSet  = std::tuple<Terms, std::string>;
    using LanguageTermSets = std::vector<LanguageTermSet>;

    struct LanguageParser
    {
        ParseFunction parser;
        std::string name;
        std::string type;
        LanguageParser(ParseFunction set_parser, std::string set_name, std::string set_type);
    };

    using LanguageParsers  = std::vector<LanguageParser>;

    struct Language
    {
        Seperators    seperators;

        LanguageTermSets language_term_sets;
        LanguageParsers  language_parsers;

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageParsers&  set_language_parsers);

        std::tuple<Token, Terms> identify(Terms terms) const;
    };
}
