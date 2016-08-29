#pragma once
#include "Consumed.hpp"
#include "../../Types/Result.hpp"

namespace Match 
{
    using MatchResult = Result<Term>; 
        /*
    struct MatchResult : public Consumed<Term>
    {
        Terms remaining;

        MatchResult(bool set_result     = false,
               Terms set_consumed  = Terms(),
               Terms set_remaining = Terms())
               : Consumed<Term>(set_result, set_consumed)
        {
            remaining = set_remaining;
        }
    };*/
}
