#include "symbol.hpp"
namespace syntax
{
string Symbol::representation(){ unordered_set<string> fake_namespace; return source(fake_namespace); }
string Symbol::source(unordered_set<string>& generated, string n_space){return "symbol";}
string Symbol::header(unordered_set<string>& generated, string n_space){return "none";};
string Symbol::name(){return "none";}
Symbol::Symbol(){}
}
