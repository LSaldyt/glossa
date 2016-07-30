#include "Language.hpp"

namespace Lex
{
    LanguageParser::LanguageParser(ParseFunction set_parser, std::string set_name, std::string set_type)
    {
        parser = set_parser;
        name   = set_name;
        type   = set_type;
    }

    Language::Language(const LanguageTermSets& set_term_sets, const LanguageParsers&  set_language_parsers) {
        language_term_sets = set_term_sets;

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

        language_parsers.insert(language_parsers.end(), set_language_parsers.begin(), set_language_parsers.end());

    }

    std::tuple<Token, Terms> Language::identify(Terms terms) const
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
}
