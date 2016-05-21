#pragma once
#include "Result.hpp"

struct NamedResult : public Result
{
    std::string name;

    NamedResult(bool set_result=false,
                Parse::Tokens set_parsed    = std::vector<std::string>(),
                Parse::Tokens set_remaining = std::vector<std::string>(),
                std::string set_name="")
        : Result(set_result, set_parsed, set_remaining)
    {
        name = set_name;
    }
};
