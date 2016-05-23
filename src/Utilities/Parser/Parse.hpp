#pragma once
#include "Types/NamedResult.hpp" //NamedResult will automatically include its subclasses (Consumed and Result)
//Types/Types.hpp is also automatically included, since the types it defines are used to build the Result types

#include "Locale.hpp"

namespace Parse
{
    using ParseFunction   = std::function<Result(Tokens)>;
    using ParseFunctions  = std::vector<ParseFunction>;
    using Consumer        = std::function<Consumed(Tokens)>;

    using Seperator       = std::tuple<char, bool>;
    using Seperators      = std::vector<Seperator>;

    std::string strip_punctuation (const std::string& sentence);
    Tokens      tokenize          (const std::string& sentence);

    ParseFunction parseTemplate(Consumer consumer);
    ParseFunction singleTemplate(TokenComparator comparator);

    ParseFunction just(std::string value);
    ParseFunction many(ParseFunction parser);

    Tokens seperate(const std::string& sentence, const Seperators &seperators);

    const ParseFunction any = singleTemplate([](Token t) { return true; } );
    const auto subsetOf = [](std::string symbols)
    {
        return singleTemplate([symbols](Token token)
        {
            return (symbols.find(token) != std::string::npos);
        });
    };

    const Seperators mathematical =
    {
    std::make_tuple(' ', false),
    std::make_tuple('+', true),
    std::make_tuple('-', true),
    std::make_tuple('*', true),
    std::make_tuple('/', true),
    std::make_tuple('(', true),
    std::make_tuple(')', true),
    std::make_tuple('^', true),
    std::make_tuple('=', true)
    };
}

// until() -parsers with a second parser (wildcard by default) until it's first parser is true, optionally returning either or both results
