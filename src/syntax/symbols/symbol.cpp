#include "symbol.hpp"
namespace syntax
{
string Symbol::representation(){ return ""; }
string Symbol::source(unordered_set<string>& generated){return "symbol";}
string Symbol::name(){return "none";}
Symbol::Symbol(){}
}
