#pragma once
#include <vector>
#include <string>


struct Result
{
    bool result;
    std::vector<std::string> parsed;
    std::vector<std::string> remaining;

    Result(bool set_result=false,
           std::vector<std::string> set_parsed    = std::vector<std::string>(),
           std::vector<std::string> set_remaining = std::vector<std::string>())
    {
        result    = set_result;
        parsed    = set_parsed;
        remaining = set_remaining;
    }
};

struct NamedResult : public Result
{
    std::string name;

    NamedResult(bool set_result=false,
                std::vector<std::string> set_parsed    = std::vector<std::string>(),
                std::vector<std::string> set_remaining = std::vector<std::string>(),
                std::string set_name="")
        : Result(set_result, set_parsed, set_remaining)
    {
        name = set_name;
    }
};
