#pragma once
#include "Import.hpp"
#include "StatementResult.hpp"

namespace Gen
{
    using StatementParser     = std::function<StatementResult(SymbolicTokens)>;

    std::tuple<bool, SymbolicTokens> advance(SymbolicTokenParser function, SymbolicTokens& tokens)
    {
        auto result = function(tokens);
        if (!result.result)
        {
            return std::make_tuple(false, SymbolicTokens());
        }
        else
        {
          tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
          return std::make_tuple(true, result.parsed);
        }
    }

    template < typename T >
    std::tuple<bool, T, SymbolicTokens> from(std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> function, const SymbolicTokens& tokens, std::function<T(SymbolicTokens)> converter)
    {
        auto result = function(tokens);
        if (!result.result)
        {
            return std::make_tuple(false, T(), SymbolicTokens());
        }
        return std::make_tuple(true, converter(result.parsed), result.parsed);
    }

    template < typename T >
    std::tuple<bool, SymbolicTokens> bind(T &t, std::tuple<bool, T, SymbolicTokens> result)
    {
        if (! std::get<0>(result))
        {
            return std::make_tuple(false, SymbolicTokens());
        }
        else
        {
          t = std::get<1>(result);
          return std::make_tuple(true, std::get<2>(result));
        }
    }
}
