#include "symbol.hpp"
namespace syntax
{
    struct String : public StringLiteral 
    {
        String(string set_value) : StringLiteral(set_value){}
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype)
        {
            return "\"" + value + "\"s";
        }
        virtual string abstract(int indent=0)
        {
            return repeatString("    ", indent) + "String(\"" + value + "\")";
        }
    };
}
