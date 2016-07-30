#pragma once
#include "../Syntax/Token.hpp"

template< typename TokenType >
struct TokenResult
{
    bool result;
    std::vector<TokenType> parsed;
    std::vector<TokenType> remaining;

    TokenResult(bool set_result              = false,
                std::vector<TokenType> set_parsed    = std::vector<TokenType>(),
                std::vector<TokenType> set_remaining = std::vector<TokenType>())
    {
        result    = set_result;
        parsed    = set_parsed;
        remaining = set_remaining;
    }
};
