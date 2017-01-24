#pragma once
#include "../import.hpp"

/**
 * Classes that represent core language features: identifiers, literals, etc
 * Defines abstract/realistic representations for each low-level syntax element
 */
namespace syntax
{
    using namespace tools;
    /**
     * Abstract base class for low-level syntax elements
     */
    struct Symbol
    {
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype, int nesting=0);
        virtual string abstract(int indent=0);
        virtual string name();

        virtual tuple<string, vector<vector<shared_ptr<Symbol>>>> to_id_group();

        string annotation = "symbol";

        Symbol();
    };

    using SymbolGenerator  = function<shared_ptr<Symbol>(vector<string>)>;
}
