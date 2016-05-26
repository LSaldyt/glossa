#pragma once
#include "../../Syntax/Token.hpp"

struct TokenResult
{
    bool result;
    Syntax::Tokens parsed;
    Syntax::Tokens remaining;

    TokenResult(bool set_result              = false,
                Syntax::Tokens set_parsed    = Syntax::Tokens(),
                Syntax::Tokens set_remaining = Syntax::Tokens())
    {
        result    = set_result;
        parsed    = set_parsed;
        remaining = set_remaining;
    }
};
