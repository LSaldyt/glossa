#pragma once
#include "Result.hpp"

namespace Parse
{
    struct NamedResult : public Result
    {
        std::string name;

        NamedResult(bool set_result=false,
                    Terms set_parsed    = Terms(),
                    Terms set_remaining = Terms(),
                    std::string set_name="")
            : Result(set_result, set_parsed, set_remaining)
        {
            name = set_name;
        }
    };
}
