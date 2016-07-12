#include "Gen.hpp"

namespace Gen
{
    bool advance(SymbolicTokenParser function, SymbolicTokens& tokens)
    {
        auto result = function(tokens);
        if (!result.result)
        {
            return false;
        }
        else
        {
            tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
            return true;
        }
    }

}
