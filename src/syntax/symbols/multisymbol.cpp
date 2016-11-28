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
            for (auto line : get<2>(file))
            {
                representation += line + " ";
            } 
        }
    }
    return representation;
}

string MultiSymbol::abstract(int indent)
{
    string representation = repeatString("    ", indent) + "MultiSymbol (" + tag + ")\n";
    for (auto group : groups)
    {
        for (auto symbol : group)
        {
            representation += symbol->abstract(indent + 1) + "\n";
        }
    }
    return representation;
}

}
