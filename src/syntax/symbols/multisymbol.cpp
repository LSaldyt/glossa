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
                if (not line.empty())
                {
                    representation += line;
                }
            } 
        }
    }
    return representation;
}

string MultiSymbol::abstract(int indent)
{
    int next_indent = indent;
    string representation;
    if (tag != "statement" and tag != "value" and tag != "basevalue")
    {
        representation = repeatString("  ", indent) + "MultiSymbol (" + tag + ")\n";
        next_indent++;
    }
    for (auto kv : table)
    {
        for (auto symbol : kv.second)
        {
            auto line = symbol->abstract(next_indent);
            if (not rtrim(line).empty())
            {
                representation += line + "\n";
            }
        }
        //representation += ",";
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
