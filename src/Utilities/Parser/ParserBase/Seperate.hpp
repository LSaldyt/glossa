#pragma once
#include "../Types/Types.hpp"

namespace Parse
{
    using Seperator       = std::tuple<char, bool>;
    using Seperators      = std::vector<Seperator>;

    std::string strip_punctuation (const std::string& sentence);
    Terms seperate(const std::string& sentence, const Seperators &seperators);

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
