#include "multisymbol.hpp"

namespace syntax
{

MultiSymbol::MultiSymbol()
{}
MultiSymbol::MultiSymbol(string set_tag, vector<vector<shared_ptr<Symbol>>> set_groups) : 
    tag(set_tag),
    groups(set_groups)
{
    annotation = "multisymbol";
}

string MultiSymbol::source(Generator& generator, unordered_set<string>& names, string n_space)
{
    string representation = "";
    auto files = generator(names, groups, tag);
    for (auto line : get<1>(files))
    {
        representation += line + " ";
    } 
    return representation;
}

string MultiSymbol::header(Generator& generator, unordered_set<string>& names, string n_space)
{
    string representation = "";
    auto files = generator(names, groups, tag);
    for (auto line : get<0>(files))
    {
        representation += line + " ";
    } 
    return representation;
}

}
