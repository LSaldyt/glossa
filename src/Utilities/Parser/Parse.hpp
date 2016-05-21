#pragma once
#include "Result.hpp"

namespace Parse
{

    std::string strip_punctuation (const std::string& sentence);
    Tokens      tokenize          (const std::string& sentence);

    ParseFunction just(std::string value);
    ParseFunction many(ParseFunction parser);
}
