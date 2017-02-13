#pragma once
#include "symbol.hpp"

namespace syntax
{
    using namespace tools;
    /**
     * Signal symbol for some hacky uses... (i.e. seperators, AST-transform matrix edge indicators) 
     */
    struct SentinelSymbol : public Symbol
    {
        string name() override;
        string abstract(int indent=0) override;

        string tag = "__none__";
        string val = "__none__";

        SentinelSymbol(string set_tag, string set_val="__none__");
        SentinelSymbol();
    };
}
