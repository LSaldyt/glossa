#pragma once
#include "Import.hpp"
#include "StatementResult.hpp"

namespace Gen
{
    using StatementParser     = std::function<StatementResult(SymbolicTokens)>;

    bool advance(SymbolicTokenParser function, SymbolicTokens& tokens);

    template < typename T >
    std::function<std::tuple<bool, T>(SymbolicTokens&)> builder (std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> function, std::function<T(SymbolicTokens)> converter)
    {
        std::function<std::tuple<bool, T>(SymbolicTokens&)> f = [function, converter](SymbolicTokens& tokens)
        {
            auto result = function(tokens);
            auto to_return = std::make_tuple(false, T());
            if (result.result)
            {
                tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
                to_return = std::make_tuple(true, converter(result.parsed));
            }
            return to_return;
        };
        return f;
    }

    template < typename T >
    bool bindTo(T &t, std::function<std::tuple<bool, T>(SymbolicTokens&)> typeGenerator, SymbolicTokens& tokens)
    {
        auto result = typeGenerator(tokens);
        if (! std::get<0>(result))
        {
            return false;
        }
        else
        {
            t = std::get<1>(result);
            return true;
        }
    }
}
