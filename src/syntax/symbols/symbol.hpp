#pragma once
#include "../import.hpp"

namespace syntax
{
    using namespace tools;
    struct Symbol
    {
        virtual string representation();
        virtual string source(unordered_set<string>& generated);
        virtual string header(unordered_set<string>& generated);
        virtual string name();

        string annotation = "symbol";

        Symbol();
    };

    using SymbolGenerator  = function<shared_ptr<Symbol>(vector<string>)>;
    using SymbolGenerators = vector<SymbolGenerator>;
}
