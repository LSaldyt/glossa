#include "symbol.hpp"
namespace syntax
{
string Symbol::representation(Generator& generator, unordered_set<string>& generated, string filetype)
{
    return "/*No Representation*/";
}
string Symbol::name(){return "none";}
Symbol::Symbol(){}
}
