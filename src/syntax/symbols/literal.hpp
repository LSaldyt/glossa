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
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype)
        {
            return std::to_string(value);
        }
        virtual string abstract(int indent=0)
        {
            return repeatString("    ", indent) + "(" + std::to_string(value) + ")";
        }
    };

    struct StringLiteral : public Symbol
    {
        string value;
        StringLiteral(string set_value) : value(set_value)
        {
        }
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype)
        {
            return value;
        }
        virtual string abstract(int indent=0)
        {
            return repeatString("    ", indent) + "(" + value + ")";
        }
        virtual string name()
        {
            return "none";
        }
    };
}
