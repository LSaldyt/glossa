#pragma once
#include "Lex/Seperate.hpp"
#include "Gen/Gen.hpp"

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
        Gen::Generator        language_generator;

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageParsers&  set_language_parsers,
                 const Gen::SymbolicStatementParsers& statement_parsers)
        {
            language_term_sets = set_term_sets;
            language_parsers   = set_language_parsers;

            seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

            for (auto term_set : language_term_sets)
            {
                language_parsers.push_back(std::make_tuple(anyOf(justFrom(std::get<0>(term_set))), std::get<1>(term_set)));
            }

            std::vector<Gen::Generator> generators;
            generators.reserve(statement_parsers.size());
            for (auto s_parser : statement_parsers)
            {
                generators.push_back(Gen::makeGenerator(s_parser));
            }

            language_generator = Gen::firstOf(generators);
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
