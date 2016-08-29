#pragma once
#include "../Types/SymbolicToken.hpp"

struct TokenResult
{
    bool result;
    SymbolicTokens consumed;
    SymbolicTokens remaining;

    TokenResult(bool set_result              = false,
                SymbolicTokens set_parsed    = SymbolicTokens(),
                SymbolicTokens set_remaining = SymbolicTokens())
    {
        result    = set_result;
        parsed    = set_parsed;
        remaining = set_remaining;
    }
};
