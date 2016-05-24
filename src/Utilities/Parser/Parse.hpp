#pragma once
#include "ParserBase/Locale.hpp"
#include "ParserBase/Seperate.hpp"
#include "ParserBase/Templates.hpp"

namespace Parse
{
    const auto just = [](std::string value)
    {
        auto comparator = [value](Term term){ return value == term; };
        return singleTemplate(comparator);
    };

    const auto wildcard = singleTemplate([](Term t) { return true; } );

    const auto subsetOf = [](std::string symbols)
    {
        return singleTemplate([symbols](Term term)
        {
            for(auto c : term)
            {
                if(symbols.empty() || (symbols.find_first_not_of(c) == std::string::npos))
                {
                    return false;
                }
            }
            return true;
        });
    };

    const auto anyOf = [](ParseFunctions functions)
    {
        return parseTemplate([functions](Terms terms)
        {
            auto result = Result(false, Terms(), terms);
            for (auto function : functions)
            {
                auto func_result = function(terms);
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
        Consumer consumer = [parser](Terms terms)
        {
            auto result = parser(terms);
            return Consumed(result.result, result.parsed);
        };
        return multiTemplate(consumer);
    };

    const auto digits = singleTemplate(is_digits);
    const auto alphas = singleTemplate(is_alphas);
    const auto puncts = singleTemplate(is_puncts);
    const auto uppers = singleTemplate(is_uppers);
    const auto lowers = singleTemplate(is_lowers);

    const auto parseOp = subsetOf("+-/*");
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
