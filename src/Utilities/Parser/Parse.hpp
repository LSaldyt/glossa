#pragma once
#include "Types/NamedResult.hpp" //NamedResult will automatically include its subclasses (Consumed and Result)
//Types/Types.hpp is also automatically included, since the types it defines are used to build the Result types

namespace Parse
{
    using ParseFunction   = std::function<Result(Tokens)>;
    using Consumer        = std::function<Consumed(Tokens)>;

    using Seperator       = std::tuple<char, bool>;
    using Seperators      = std::vector<Seperator>;

    std::string strip_punctuation (const std::string& sentence);
    Tokens      tokenize          (const std::string& sentence);

    ParseFunction just(std::string value);
    ParseFunction many(ParseFunction parser);

    Tokens seperate(const std::string& sentence, const Seperators &seperators);
}
