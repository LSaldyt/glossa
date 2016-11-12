#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    tuple<vector<string>, vector<string>> generateFiles(string filename, vector<shared_ptr<Symbol>>& symbols)
    {
        vector<string> header;
        header.push_back("#pragma once\n");
        header.push_back("#include \"../std/std.hpp\"");
        concat(header, generateHeader(symbols));

        vector<string> source;
        source.push_back("#include \"" + filename + ".hpp\""); 
        concat(source, generate(symbols));

        return make_tuple(source, header);
    }

    vector<string> generate(vector<shared_ptr<Symbol>>& symbols, unordered_set<string> outer_names, string n_space)
    {
        vector<string> output;
        output.reserve(symbols.size());

        unordered_set<string> names(outer_names);

        for (auto s : symbols)
        {
            print("Generated:");
            print(s->source(names, n_space) + ";");
            output.push_back(s->source(names, n_space) + ";\n");
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
            }
        }
        return output;
    }

    vector<string> generateHeader(vector<shared_ptr<Symbol>>& symbols, unordered_set<string> outer_names)
    {
        vector<string> output;
        output.reserve(symbols.size());

        unordered_set<string> names(outer_names);

        for (auto s : symbols)
        {
            print("Generated:");
            print(s->header(names) + ";");
            output.push_back(s->header(names) + ";\n");
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
            }
        }
        return output;
    }

    string buildArglist(vector<string>& arglist)
    {
        string args_representation = "";
        for (int i =0; i < arglist.size(); i++)
        { 
            args_representation += ("auto " + arglist[i]);
            if (i+1 != arglist.size()) //If not on last iteration
            {
                args_representation += ", ";
            }
        }
        return args_representation;
    }

}
