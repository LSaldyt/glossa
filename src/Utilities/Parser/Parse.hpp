#pragma once
#include "Result.hpp"
#include <functional>

namespace Parse
{
    using ParseFunction   = std::function<Result(std::vector<std::string>)>;
    using TokenComparator = std::function<bool(std::string)>;
    using Tokens          = std::vector<std::string>;
    using Token           = std::string;

    std::string strip_punctuation (const std::string& sentence);
    Tokens      tokenize          (const std::string& sentence);

    ParseFunction singleTemplate(TokenComparator comparator);

    ParseFunction just(std::string value);
}
