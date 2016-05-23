#pragma once
#include "ParserBase/Locale.hpp"
#include "ParserBase/Seperate.hpp"
#include "ParserBase/Templates.hpp"

namespace Parse
{
    const auto just = [](std::string value)
    {
        auto comparator = [value](std::string token){ return value == token; };
        return singleTemplate(comparator);
    };

    const auto wildcard = singleTemplate([](Token t) { return true; } );

    const auto subsetOf = [](std::string symbols)
    {
        return singleTemplate([symbols](Token token)
        {
            return (symbols.find(token) != std::string::npos);
        });
    };

    const auto anyOf = [](ParseFunctions functions)
    {
        return parseTemplate([functions](Tokens tokens)
        {
            auto result = Result(false, Tokens(), tokens);
            for (auto function : functions)
            {
                auto func_result = function(tokens);
                if(func_result.result)
                {
                    result = func_result;
                }
            }
            return result;
        });
    };

    const auto many = [](ParseFunction parser)
    {
        Consumer consumer = [parser](Tokens tokens)
        {
            auto result = parser(tokens);
            return Consumed(result.result, result.parsed);
        };
        return multiTemplate(consumer);
    };


    const auto digits = singleTemplate(is_digits);
    const auto alphas = singleTemplate(is_alphas);
    const auto puncts = singleTemplate(is_puncts);
    const auto uppers = singleTemplate(is_uppers);
    const auto lowers = singleTemplate(is_lowers);
}

// Single Token Parsers:
// just()     -returns a function that parses a string exactly,
// wildcard() -parses single string (non whitespace),
// noneOf()   -takes a list of parsers and tries them against a single string
// allOf()    -does the same, but ensuring all parsers pass
// subsetOf() -takes any container of characters and ensures that the string consists entirely of those characters
// parseAny() -returns the result of the first matching parser out of a list of parsers it is given
// Multi Token Parsers:
// many()  -repeats a single parser until it fails, but returns true no matter what
// until() -parsers with a second parser (wildcard by default) until it's first parser is true, optionally returning either or both results
