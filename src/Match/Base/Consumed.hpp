#pragma once
#include "Types.hpp"

namespace Match 
{

    struct Consumed
    {
        bool   result;
        Terms parsed;

        Consumed(bool set_result = false,
                 Terms set_parsed = Terms())
        {
            result = set_result;
            parsed = set_parsed;
        }
    };
}
