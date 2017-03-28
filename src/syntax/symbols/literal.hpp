#pragma once
#include "../import.hpp"
#include "symbol.hpp"

namespace syntax
{
    /**
     * Abstract representation of a literal
     */
    template < typename T>
    struct Literal : public Symbol
    {
        T value;
        Literal(T set_value)
        {
            value = set_value; 
        }
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype, int nesting=0)
        {
            return std::to_string(value);
        }
        virtual string abstract(int indent=0)
        {
            return repeatString("  ", indent) + "(" + std::to_string(value) + ")";
        }
    };

    /**
     * Abstract representation of a string literal
     */
    struct StringLiteral : public Symbol
    {
        string value;
        StringLiteral(string set_value) : value(set_value)
        {
        }
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype, int nesting=0)
        {
            return value;
        }
        virtual string abstract(int indent=0)
        {
            return repeatString("  ", indent) + "(" + value + ")";
        }
        virtual string name()
        {
            return "none";
        }
    };
}
