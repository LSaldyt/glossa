#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    string format(const string& inner, const string& formatter)
    {
        string representation(formatter);
        replaceAll(representation, "@", inner);
        return representation;
    }

    string sepWith(Generator& generator, const vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string filetype, string sep, string formatter)
    {
        string line = "";
        for (int i = 0; i < symbols.size(); i++)
        { 
            string inner_representation = symbols[i]->representation(generator, names, filetype);
            line += format(inner_representation, formatter);

            if (i+1 != symbols.size()) //If not on last iteration
            {
                line += sep;
            }
        }
        return line;
    }
}
