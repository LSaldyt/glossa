#include "identifier.hpp"

namespace syntax
{

Identifier::Identifier(string set_value) : StringLiteral(set_value){}

string Identifier::name(){return value;}
string Identifier::source(Generator& generator, unordered_set<string>& generated, string n_space)
{
    return value;
}
string Identifier::header(Generator& generator, unordered_set<string>& generated, string n_space)
{
    return value;
}

}
