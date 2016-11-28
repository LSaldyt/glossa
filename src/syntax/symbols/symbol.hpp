#pragma once
#include "../import.hpp"

namespace syntax
{
    using namespace tools;
    struct Symbol
    {
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype);
        virtual string abstract(int indent=0);
        virtual string name();

        string annotation = "symbol";

        Symbol();
    };

    using SymbolGenerator  = function<shared_ptr<Symbol>(vector<string>)>;
    using SymbolGenerators = vector<SymbolGenerator>;
}
