#pragma once
#include "Types.hpp"

namespace Match 
{
    struct Consumed
    {
        bool  result;
        Terms consumed;

        Consumed(bool set_result    = false,
                 Terms set_consumed = Terms())
        {
            result = set_result;
            consumed = set_consumed;
        }
    };
}
