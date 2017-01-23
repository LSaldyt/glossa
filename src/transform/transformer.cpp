#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace transform 
{

/**
 * Constructs a transformer 
 * @param filenames Construction files
 * @param directory Directory that construction files reside in
 */
TransformerMap::TransformerMap(vector<string> filenames, string directory)
{
    for (auto filename : filenames)
    {
        print("Reading transformer " + directory + filename);
        readTransformerFile(directory + filename);
    }
}

void TransformerMap::operator()(IdentifiedGroups& identified_groups)
{
    print("Performing transformation of identified groups");
    for (auto& group : identified_groups)
    {
        auto& tag    = get<0>(group);
        auto& matrix = get<1>(group);
        for (auto transformer_tuple : transformers)
        {
            auto name                     = get<0>(transformer_tuple);
            auto transformer              = get<1>(transformer_tuple);
            auto symbol_storage_generator = get<2>(transformer_tuple);
            auto symbol_storage           = symbol_storage_generator(matrix);
            if (name == tag)
            {
                print("Transforming tagged group " + tag);
                //auto transform_result = transformer(matrix, symbol_storage);
                //tag    = get<0>(transform_result);
                //matrix = get<1>(transform_result);
            }
        }
    }
}

void TransformerMap::readTransformerFile(string filename)
{
    auto content = readFile(filename);
    
    // Seperate constructor into header and source constructors
    assert(contains(content, "name"s));
    auto name_i = std::find(content.begin(), content.end(), "name");
    assert(name_i + 1 != content.end());
    auto name         = *(name_i + 1);
    auto declarations = vector<string>(content.begin(), name_i);
    auto body         = vector<string>(name_i + 2, content.end());

    transformers.push_back(make_tuple(name, readTransformer(body), generateSymbolStorageGenerator(declarations)));
}

Transformer TransformerMap::readTransformer(vector<string> content)
{
    string name = "statement";
    if (content.size() > 1)
    {
        name    = content[0];
        content = slice(content, 1);
    }
    return [content, name](SymbolMatrix matrix, SymbolStorage& symbol_storage){
        SymbolMatrix new_matrix;
        /*
        for (auto line : content)
        {
            vector<shared_ptr<Symbol>> new_row;
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            for (auto term : terms)
            {
                bool single_elem = contains(get<0>(symbol_storage), line);
                if (single_elem)
                {
                    new_row.push_back(get<0>(symbol_storage)[line]);
                }
                else
                {
                    assert(contains(get<1>(symbol_storage), line));
                    concat(new_row, get<1>(symbol_storage)[line]);
                }
            }
            new_matrix.push_back(new_row);
        }
        */
        return make_tuple(name, new_matrix);
    };
}

}
