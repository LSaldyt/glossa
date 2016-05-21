#pragma once
#include "Consumed.hpp"

struct Result : public Consumed
{
    Parse::Tokens remaining;

    Result(bool set_result=false,
           Parse::Tokens set_parsed    = std::vector<std::string>(),
           Parse::Tokens set_remaining = std::vector<std::string>())
           : Consumed(set_result, set_parsed)
    {
        remaining = set_remaining;
    }
};
