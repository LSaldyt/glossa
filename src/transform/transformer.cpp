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
        readTransformerBody(directory + filename);
    }
}

IdentifiedGroups TransformerMap::operator()(IdentifiedGroups identified_groups)
{
    return identified_groups;
}

void TransformerMap::readTransformerBody(string filename)
{
    auto content = readFile(filename);
    
    // Seperate constructor into header and source constructors
    assert(contains(content, "name"s));
    auto name_i = std::find(content.begin(), content.end(), "name");
    assert(name_i + 1 != content.end());
    auto name         = *(name_i + 1);
    auto declarations = vector<string>(content.begin(), name_i);
    auto body         = vector<string>(name_i + 2, content.end());

    transformers.push_back(make_tuple(name, readTransformer(body)));
}

Transformer TransformerMap::readTransformer(vector<string> content)
{
    return [](SymbolMatrix matrix, SymbolStorage& symbol_storage){
        return matrix;
    };
}

}
