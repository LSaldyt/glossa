#include "symbol.hpp"
namespace syntax
{
string Symbol::representation(Generator& generator, unordered_set<string>& generated, string filetype, int nesting)
{
    return "/*No Representation*/";
}
string Symbol::abstract(int indent)
{
    return "Symbol";
}
string Symbol::name(){return "none";}
Symbol::Symbol(){}
}
