#include "symbol.hpp"
namespace syntax
{
    struct String : public StringLiteral 
    {
        String(string set_value) : StringLiteral(set_value){}
        string representation(Generator& generator, unordered_set<string>& generated, string filetype)
        {
            return "\"" + value + "\"s";
        }
    };
}
