#pragma once

template <typename T>
struct Result
{
    bool result;
    std::string annotation;
    std::vector<T> consumed;
    std::vector<T> remaining;
    
    Result(bool set_result=false,
           std::vector<T> set_consumed=std::vector<T>(),
           std::vector<T> set_remaining=std::vector<T>()) :
       annotation("none") 
    {
        result    = set_result;
        consumed  = set_consumed;
        remaining = set_remaining;
    }
};


