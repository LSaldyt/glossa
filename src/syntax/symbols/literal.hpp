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
        virtual string source(unordered_set<string>& names, string n_space="")
        {
            return std::to_string(value);
        }
        virtual string header(unordered_set<string>& names, string n_space="")
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
        virtual string source(unordered_set<string>& names, string n_space="")
        {
            return value;
        }
        virtual string header(unordered_set<string>& names, string n_space="")
        {
            return value;
        }
        virtual string name()
        {
            return "none";
        }
    };
}
