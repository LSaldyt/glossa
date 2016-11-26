#pragma once
#include "../import.hpp"

namespace syntax
{
    using namespace tools;
    struct Symbol
    {
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype);
        virtual string representation();
        virtual string source(unordered_set<string>& generated, string n_space="");
        virtual string header(unordered_set<string>& generated, string n_space="");
        virtual string source(Generator& generator, unordered_set<string>& generated, string n_space="");
        virtual string header(Generator& generator, unordered_set<string>& generated, string n_space="");
        virtual string name();

        string annotation = "symbol";

        Symbol();
    };

    using SymbolGenerator  = function<shared_ptr<Symbol>(vector<string>)>;
    using SymbolGenerators = vector<SymbolGenerator>;
}
