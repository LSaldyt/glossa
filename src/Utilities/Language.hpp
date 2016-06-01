#pragma once
#include "Lex/Seperate.hpp"
#include "Gen/Gen.hpp"

namespace Lex
{
    using LanguageTermSet  = std::tuple<Terms, std::string>;
    using LanguageTermSets = std::vector<LanguageTermSet>;

    struct LanguageParser
    {
        ParseFunction parser;
        std::string name;
        std::string type;
        LanguageParser(ParseFunction set_parser, std::string set_name, std::string set_type)
        {
            parser = set_parser;
            name   = set_name;
            type   = set_type;
        }
    };

    using LanguageParsers  = std::vector<LanguageParser>;

    struct Language
    {
        Seperators    seperators;

        LanguageTermSets language_term_sets;
        LanguageParsers  language_parsers;
        Gen::Generator   language_generator;

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageParsers&  set_language_parsers,
                 const Gen::SymbolicStatementParsers& statement_parsers)
        {
            language_term_sets = set_term_sets;
            language_parsers   = set_language_parsers;

            seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

            for (auto term_set : language_term_sets)
            {
                for (auto term : std::get<0>(term_set))
                {
                    language_parsers.push_back(LanguageParser(just(term), term, std::get<1>(term_set)));
                    if(std::get<1>(term_set) != "keyword") //seperating by keywords would make identifiers containing keywords impossible
                    {
                        seperators.push_back(std::make_tuple(term, true));
                    }
                }
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
                auto result = parser.parser(terms);
                if(result.result)
                {
                    return std::make_tuple(Token(result.parsed[0], parser.name, parser.type), result.remaining);
                }
            }
            return std::make_tuple(Token("", "", "failure"), Terms());
        }
    };
}
