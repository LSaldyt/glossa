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
    }
}

IdentifiedGroups TransformerMap::operator()(IdentifiedGroups identified_groups)
{
    return identified_groups;
}

}
