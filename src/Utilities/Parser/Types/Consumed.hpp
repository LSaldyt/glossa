#pragma once
#include "Types.hpp"

struct Consumed
{
    bool   result;
    Parse::Tokens parsed;

    Consumed(bool set_result = false,
             Parse::Tokens set_parsed = std::vector<std::string>())
    {
        result = set_result;
        parsed = set_parsed;
    }
};
