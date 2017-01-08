#pragma once
#include "import.hpp"

/**
 * Generic class representing the result of a parse/match attempt
 */
template <typename T>
struct Result
{
    bool result;
    tools::vector<T> consumed;
    tools::vector<T> remaining;
    
    Result(bool set_result=false,
           tools::vector<T> set_consumed=tools::vector<T>(),
           tools::vector<T> set_remaining=tools::vector<T>()) 
    {
        result    = set_result;
        consumed  = set_consumed;
        remaining = set_remaining;
    }
};


