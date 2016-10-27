#include "symbol.hpp"
namespace syntax
{
    struct String : public StringLiteral 
    {
        String(string set_value) : StringLiteral(set_value){}
        string representation()
        {
            return "String: \"" + value + "\"";
        }
        string source(unordered_set<string>& names, string n_space="")
        {
            return "\"" + value + "\"s";
        }
    };
}
