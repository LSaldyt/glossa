#pragma once
#include "Import.hpp"
#include "StatementResult.hpp"

namespace Gen
{
    using StatementParser     = std::function<StatementResult(SymbolicTokens)>;

    template < typename T >
    std::tuple<bool, T> from(SymbolicTokenParser function, SymbolicTokens& tokens, std::function<T(SymbolicTokens)> converter)
    {
        auto result = function(tokens);
        if (!result.result)
        {
            return std::make_tuple(false, T());
        }
        tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
        return std::make_tuple(true, converter(result.parsed));
    }

    template < typename T >
    bool bind(T t&, std::tuple<bool, T> result)
    {
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
