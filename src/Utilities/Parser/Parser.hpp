#pragma once
#include <string>
#include <vector>
#include "Parse.hpp"

template <typename T>
class Parser
{
public:

    std::vector<T> parsers;
    std::string name;
    Parse::Seperators seperators;

    Parser()
    {
    }

    Parser(const std::vector<T>& set_parsers,
           const std::string& set_name,
           const Parse::Seperators& set_seperators)
    {
        parsers    = set_parsers;
        name       = set_name;
        seperators = set_seperators;
    }
    ~Parser(){}


    NamedResult parse(const std::string& sentence)
    {
        return parse(Parse::seperate(sentence, seperators));
    }

    NamedResult parse(const std::vector<std::string>& original_tokens)
    {
        std::vector<std::string> parsed;
        std::vector<std::string> tokens = original_tokens;

        for (auto parser : parsers)
        {
            auto parse_result = parser(tokens);
            if(parse_result.result)
            {
                parsed.insert( parsed.end(), parse_result.parsed.begin(), parse_result.parsed.end() );
                tokens  = parse_result.remaining;
            }
            else
            {
                return NamedResult(false, parsed, tokens, name);
            }
        }
        return NamedResult(true, parsed, tokens, name);
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
