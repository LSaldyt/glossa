#pragma once
#include "../Types/SymbolicToken.hpp"
#include "../Types/Result.hpp"

using TokenResult = Result<SymbolicToken>;

/*
struct TokenResult
{
    bool result;
    SymbolicTokens consumed;
    SymbolicTokens remaining;

    TokenResult(bool set_result              = false,
                SymbolicTokens set_consumed  = SymbolicTokens(),
                SymbolicTokens set_remaining = SymbolicTokens())
    {
        result    = set_result;
        consumed  = set_consumed;
        remaining = set_remaining;
    }
};
*/
