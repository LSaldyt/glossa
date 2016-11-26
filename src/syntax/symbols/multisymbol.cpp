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

string MultiSymbol::representation(Generator& generator, unordered_set<string>& names, string filetype)
{
    string representation = "";
    auto files = generator(names, groups, tag);
    for (auto file : files)
    {
        auto type = get<0>(file);
        if (type == filetype)
        {
            for (auto line : get<1>(file))
            {
                representation += line + " ";
            } 
        }
    }
    return representation;
}

string MultiSymbol::source(Generator& generator, unordered_set<string>& names, string n_space)
{
    string representation = "";
    auto files = generator(names, groups, tag);
    for (auto line : get<1>(files[1]))
    {
        representation += line + " ";
    } 
    return representation;
}

string MultiSymbol::header(Generator& generator, unordered_set<string>& names, string n_space)
{
    string representation = "";
    auto files = generator(names, groups, tag);
    for (auto line : get<1>(files[0]))
    {
        representation += line + " ";
    } 
    return representation;
}

}
