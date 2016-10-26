#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    vector<string> generate(vector<shared_ptr<Symbol>>& symbols, unordered_set<string> outer_names)
    {
        vector<string> output;
        output.reserve(symbols.size());

        unordered_set<string> names(outer_names);

        for (auto s : symbols)
        {
            print("Generated:");
            print(s->source(names) + ";");
            output.push_back(s->source(names) + ";\n");
            auto name = s->name();
            if (name != "none")
            { 
                if (name[0] == '*')
                {
                    name = sliceString(name, 1);
                    print("Added name to scope: " + name);
                    names.insert(name);
                    print(names.size());
                }
                else
                {
                    checkName(name, names);
                }
            }
        }
        return output;
    }

    void checkName(string name, const unordered_set<string>& names)
    {
        if (name != "none")
        {
            auto got = names.find(name);
            if ( got == names.end() )
            {
                for (auto n : names)
                {
                    print("Scope contained: " + n);
                }
                throw named_exception(name + " is not defined");
            }
            else
            {
                print("Using defined name " + name);
            }
        }
        else
        {
            print("no name defined");
        }
    }
}
