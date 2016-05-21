#pragma once
#include <vector>
#include <string>
#include <functional>

namespace Parse
{
    using TokenComparator = std::function<bool(std::string)>;
    using Tokens          = std::vector<std::string>;
    using Token           = std::string;
}

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

namespace Parse
{
    using ParseFunction   = std::function<Result(Tokens)>;
    using Consumer        = std::function<Consumed(Tokens)>;
}
