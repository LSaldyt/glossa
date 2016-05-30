#pragma once
#include "Import.hpp"
#include <string>
namespace Gen
{
    struct GenResult
    {
        bool result;
        std::string generated;
        SymbolicTokens remaining;
        GenResult(bool set_result=false,
                     std::string set_generated = "",
                     SymbolicTokens set_remaining = SymbolicTokens())
        {
            result = set_result;
            generated = set_generated;
            remaining = set_remaining;
        }
    };

    using Generator = std::function<GenResult(SymbolicTokens)>;
}
