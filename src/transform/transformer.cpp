#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace transform 
{
ElementConstructor<shared_ptr<Symbol>> ec = [](unordered_set<string>& names,
              SymbolStorage& symbol_storage,
              string,
              vector<string>& definitions,
              int nesting,
              OutputManager logger){
        return make_shared<Symbol>(Symbol());
        };
ElementConstructorCreator<shared_ptr<Symbol>> ec_creator = 
                [](string s){
                return ec;
                };

Transformer::Transformer(vector<string> transformer_files, string directory)
{
    for (auto file : transformer_files)
    {
        auto content = readFile(directory + file);
        auto constructor = generateTransformConstructor<shared_ptr<Symbol>>(content,
                ec_creator
                );
        transformation_map[file] = constructor;
    }
}

void Transformer::operator()(IdentifiedGroups& identified_groups)
{
    print("Modifying identifed groups");
    for (auto& id_group : identified_groups)
    {
        auto& tag = get<0>(id_group);
        auto& mx  = get<1>(id_group);
        transform(tag, mx);
    }
}

void Transformer::transform(string& tag, SymbolMatrix& symbol_matrix)
{
    for (auto kv : transformation_map)
    {
        if (kv.first == tag)
        {
            print("Transforming " + tag);
            tag = tag + "_transformed";
        }
        else
        {
            print("Did not transform " + tag);
        }
    }
    for (auto& row : symbol_matrix)
    {
        for (auto& symbol : row)
        {
            auto id_group = symbol->to_id_group(); 
            auto& tag = get<0>(id_group);
            auto& mx  = get<1>(id_group);
            if (tag != "undefined")
            {
                transform(tag, mx);
                symbol->modify_id_group(tag, mx);
            }
        }
    }
}

}
