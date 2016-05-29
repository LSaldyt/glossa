#pragma once
#include "Seperate.hpp"

namespace Lex
{
    using LanguageTermSet  = std::tuple<Terms, std::string>;
    using LanguageTermSets = std::vector<LanguageTermSet>;
    using LanguageParser   = std::tuple<ParseFunction, std::string>;
    using LanguageParsers  = std::vector<LanguageParser>;

    struct Language
    {
        Seperators    seperators;

        LanguageTermSets language_term_sets;
        LanguageParsers  language_parsers;

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageParsers&  set_parsers)
        {
            language_term_sets = set_term_sets;
            language_parsers   = set_parsers;

            seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

            for (auto term_set : language_term_sets)
            {
                language_parsers.push_back(std::make_tuple(justFrom(std::get<0>(term_set)), std::get<1>(term_set)));
            }
        }

        std::tuple<Token, Terms> identify(Terms terms) const
        {
            for (auto parser : language_parsers)
            {
                auto result = std::get<0>(parser)(terms);
                if(result.result)
                {
                    return std::make_tuple(std::make_tuple(result.parsed[0], std::get<1>(parser)), result.remaining);
                }
            }
            return std::make_tuple(std::make_tuple("", "failure"), Terms());
        }
    };
}
