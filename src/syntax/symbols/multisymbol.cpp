#include "multisymbol.hpp"

namespace syntax
{

MultiSymbol::MultiSymbol()
{}
MultiSymbol::MultiSymbol(string set_tag, MultiSymbolTable set_table) : 
    tag(set_tag),
    table(set_table)
{
    annotation = "multisymbol";
}

string MultiSymbol::representation(Generator& generator, unordered_set<string>& names, string filetype, int nesting)
{
    string representation = "";
    auto files = generator(names, table, tag);
    for (auto file : files)
    {
        auto type = get<0>(file);
        if (type == filetype)
        {
            for (auto line : get<2>(file))
            {
                representation += line;
            } 
        }
    }
    return representation;
}

string MultiSymbol::abstract(int indent)
{
    string representation = repeatString("    ", indent) + "MultiSymbol (" + tag + ")\n";
    for (auto kv : table)
    {
        for (auto symbol : kv.second)
        {
            representation += symbol->abstract(indent + 1) + "\n";
        }
        representation += ",";
    }
    return representation;
}

tuple<string, MultiSymbolTable> MultiSymbol::to_id_group()
{
    return make_tuple(tag, table);
}
void MultiSymbol::modify_id_group(string set_tag, MultiSymbolTable set_table)
{
    tag = set_tag;
    table = set_table;
}

}
