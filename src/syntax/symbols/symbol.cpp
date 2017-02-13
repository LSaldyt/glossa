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
tuple<string, vector<vector<shared_ptr<Symbol>>>> Symbol::to_id_group()
{
    return make_tuple("undefined", vector<vector<shared_ptr<Symbol>>>());
}
void Symbol::modify_id_group(string, vector<vector<shared_ptr<Symbol>>>)
{
    throw std::exception();
}
}
