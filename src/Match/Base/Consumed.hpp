#pragma once
#include "Types.hpp"

namespace Match 
{
    template <typename T>
    struct Consumed
    {
        bool  result;
        std::string annotation;
        std::vector<T> consumed;

        Consumed(bool set_result    = false,
                 std::vector<T> set_consumed = std::vector<T>(),
                 std::string set_annotation  = "none"
                )
        {
            result   = set_result;
            consumed = set_consumed;
            annotation = set_annotation;
        }
    };
}
