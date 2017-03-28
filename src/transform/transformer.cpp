#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace transform 
{
ElementConstructorCreator<vector<string>> ec_creator = [](string s)
{
    ElementConstructor<vector<string>> ec;
    ec = [s](unordered_set<string>& names,
              MultiSymbolTable& ms_table,
              string filename,
              vector<string>& definitions,
              int nesting,
              OutputManager logger)
    {
        vector<string> terms = lex::seperate(s, {make_tuple(" ", false)});
        return terms;
    };
    return ec;
};

Transformer::Transformer(vector<string> transformer_files, string directory)
{
    for (auto file : transformer_files)
    {
        auto content = readFile(directory + file);
        auto constructor = generateTransformConstructor<vector<string>>(content,
                ec_creator
                );
        transformation_map[file] = constructor;
    }
}

Transformer::Transformer()
{
}

void Transformer::operator()(IdentifiedGroups& identified_groups)
{
    for (auto& id_group : identified_groups)
    {
        auto& tag      = get<0>(id_group);
        auto& ms_table = get<1>(id_group);
        transform(tag, ms_table);
    }
}

void Transformer::transform(string& tag, MultiSymbolTable& ms_table)
{
    for (auto kv : transformation_map)
    {
        if (kv.first == tag)
        {
            print("Transforming " + tag);
            unordered_set<string> names;
            auto terms = kv.second(names, 
                                   ms_table, 
                                   "none"); 
        }
    }
    for (auto& kv : ms_table)
    {
        for (auto& symbol : kv.second)
        {
            auto id_group  = symbol->to_id_group(); 
            auto& tag      = get<0>(id_group);
            auto& ms_table = get<1>(id_group);
            if (tag != "undefined")
            {
                transform(tag, ms_table);
                symbol->modify_id_group(tag, ms_table);
            }
        }
    }
}

}
