#pragma once
#include <string>
#include <vector>
#include "Parse.hpp"

namespace Parse
{
    template <typename T>
    class Parser
    {
    public:

        std::vector<T> parsers;
        std::string name;
        Seperators seperators;

        Parser()
        {
        }

        Parser(const std::vector<T>& set_parsers,
               const std::string& set_name,
               const Seperators& set_seperators)
        {
            parsers    = set_parsers;
            name       = set_name;
            seperators = set_seperators;
        }
        ~Parser(){}


        NamedResult parse(const std::string& sentence)
        {
            return parse(seperate(sentence, seperators));
        }

        NamedResult parse(const Terms& original_terms)
        {
            Terms parsed;
            Terms terms = original_terms;

            for (auto parser : parsers)
            {
                auto parse_result = parser(terms);
                if(parse_result.result)
                {
                    parsed.insert( parsed.end(), parse_result.parsed.begin(), parse_result.parsed.end() );
                    terms  = parse_result.remaining;
                }
                else
                {
                    return NamedResult(false, Terms(), original_terms, name);
                }
            }
            return NamedResult(true, parsed, terms, name);
        }


        NamedResult operator()(const std::string& sentence)
        {
            return parse(sentence);
        }

        NamedResult operator()(const std::vector<std::string>& tokens)
        {
            return parse(tokens);
        }
    };
}
