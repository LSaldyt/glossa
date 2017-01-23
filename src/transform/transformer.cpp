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
    for (auto& group : identified_groups)
    {
        auto  tag    = get<0>(group);
        auto& matrix = get<1>(group);
        for (auto transformer_tuple : transformers)
        {
            auto name                     = get<0>(transformer_tuple);
            auto transformer              = get<1>(transformer_tuple);
            auto symbol_storage_generator = get<2>(transformer_tuple);
            auto symbol_storage           = symbol_storage_generator(matrix);
            if (name == tag)
            {
                matrix = transformer(matrix, symbol_storage);
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
    return [](SymbolMatrix matrix, SymbolStorage& symbol_storage){
        return matrix;
    };
}

}
