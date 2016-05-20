#pragma once
#include <string>
#include <vector>
#include "Parse.hpp"
#include "Result.hpp"


//typedef Result (*ParseFunction)(std::vector<std::string> tokens);

//std::vector<ParseFunction> functions;

//Result test(std::vector<std::string> tokens);

//functions.append(test);


class Parser
{
public:
    Parser(/*parsers=[], name=""*/){}
    ~Parser(){}

    std::vector<Parser> parsers;
    std::string name;

    NamedResult parse(const std::string& sentence)
    {
        return parse(tokenize(sentence));
    }

    NamedResult parse(const std::vector<std::string>& original_tokens)
    {
        std::vector<std::string> parsed;
        std::vector<std::string> tokens = original_tokens;

        for (auto parser : parsers)
        {
            auto parse_result = parser.parse(tokens);
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
