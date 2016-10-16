#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    vector<string> generate(vector<shared_ptr<Symbol>>& symbols)
    {
        vector<string> output;
        output.reserve(symbols.size());


        unordered_set<string> names;

        for (auto s : symbols)
        {
            print("Generated:");
            print(s->source(names) + ";");
            output.push_back(s->source(names) + ";");
            if (s->name() != "none")
            { 
                print("Added name to scope: " + s->name());
                names.insert(s->name());
            }
        }
        return output;
    }
}
