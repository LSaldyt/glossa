#pragma once
#include "Types.hpp"

namespace Match 
{
    template <typename T>
    struct Consumed
    {
        bool  result;
        std::vector<T> consumed;

        Consumed(bool set_result    = false,
                 std::vector<T> set_consumed = std::vector<T>())
        {
            result   = set_result;
            consumed = set_consumed;
        }
    };
}
