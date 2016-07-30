#pragma once
#include "Consumed.hpp"

namespace Match 
{
    struct Result : public Consumed
    {
        Terms remaining;

        Result(bool set_result=false,
               Terms set_parsed    = Terms(),
               Terms set_remaining = Terms())
               : Consumed(set_result, set_parsed)
        {
            remaining = set_remaining;
        }
    };
}
