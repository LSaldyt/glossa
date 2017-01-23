#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace gen 
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
        //readConstructor(directory + filename);
    }
}

}
