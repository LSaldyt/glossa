#pragma once
#include "Consumed.hpp"

namespace Match 
{
    struct Result : public Consumed
    {
        Terms remaining;

        Result(bool set_result=false,
               Terms set_consumed    = Terms(),
               Terms set_remaining = Terms())
               : Consumed(set_result, set_consumed)
        {
            remaining = set_remaining;
        }
    };
}
