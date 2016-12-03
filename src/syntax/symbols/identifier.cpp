#include "identifier.hpp"

namespace syntax
{

Identifier::Identifier(string set_value) : StringLiteral(set_value){}

string Identifier::name(){return value;}
string Identifier::representation(Generator& generator, unordered_set<string>& generated, string filetype)
{
    return value;
}
string Identifier::abstract(int indent)
{
    return repeatString("    ", indent) + "Identifier (" + value + ")";
}

}
