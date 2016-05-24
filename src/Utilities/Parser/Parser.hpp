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
        Seperators seperators;

        Parser()
        {
        }

        Parser(const std::vector<T>& set_parsers,
               const Seperators& set_seperators)
        {
            parsers    = set_parsers;
            seperators = set_seperators;
        }
        ~Parser(){}


        Result parse(const std::string& sentence)
        {
            return parse(seperate(sentence, seperators));
        }

        Result parse(const Terms& original_terms)
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
                    return Result (false, Terms(), original_terms);
                }
            }
            return Result(true, parsed, terms);
        }


        Result operator()(const std::string& sentence)
        {
            return parse(sentence);
        }

        Result operator()(const std::vector<std::string>& tokens)
        {
            return parse(tokens);
        }
    };
}
