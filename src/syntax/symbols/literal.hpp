#pragma once
#include "../import.hpp"
#include "symbol.hpp"

namespace syntax
{
    template < typename T>
    struct Literal : public Symbol
    {
        T value;
        Literal(T set_value)
        {
            value = set_value; 
        }
        virtual string source(unordered_set<string>& names)
        {
            return std::to_string(value);
        }

    };

    struct StringLiteral : public Symbol
    {
        string value;
        StringLiteral(string set_value) : value(set_value)
        {
        }
        virtual string source(unordered_set<string>& names)
        {
            return value;
        }
    };
}
