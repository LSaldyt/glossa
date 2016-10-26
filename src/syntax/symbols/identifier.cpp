#include "identifier.hpp"

namespace syntax
{

Identifier::Identifier(string set_value) : StringLiteral(set_value){}

string Identifier::name(){return value;}

}
