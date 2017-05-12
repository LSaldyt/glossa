/// Copyright 2017 Lucas Saldyt
#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    /**
     * Replaces @ characters with any string
     * @param inner Replacement string
     * @param formatter String containing @ symbols
     * @return formatted string
     */
    string format(const string& inner, const string& formatter)
    {
        string representation(formatter);
        replaceAll(representation, "@", inner);
        return representation;
    }

    /**
     * Helper function for generating delimited source code from symbols
     * @param generator Generator for a particular language
     * @param symbols   vector of symbols to be formatted
     * @param names     Namespace
     * @param filetype  Type of file to be generated
     * @param sep       String to seperate generated code with
     * @param formatter String to format each element with
     * @param nesting   Indentation level
     * @return Formatted string representing delimited constructed syntax elements
     */
    string sepWith(Generator& generator, const vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string filetype, string sep, string formatter, int nesting)
    {
        string line = "";
        for (int i = 0; i < symbols.size(); i++)
        { 
            string inner_representation = symbols[i]->representation(generator, names, filetype, nesting);
            line += format(inner_representation, formatter);

            if (i+1 != symbols.size()) //If not on last iteration
            {
                line += sep;
            }
        }
        return line;
    }
}
