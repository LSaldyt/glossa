#include "gen.hpp"
#include "../syntax/symbols/export.hpp"

namespace gen
{
    tuple<vector<string>, vector<string>> generateFiles(string filename, vector<shared_ptr<Symbol>>& symbols, Generator& generator)
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
            auto repr = s->source(names, n_space);
            if (not repr.empty())
            {
                repr += ";";
                print(repr);
                output.push_back(repr + "\n");
                auto name = s->name();
                if (name != "none")
                { 
                    if (name[0] == '*')
                    {
                        name = sliceString(name, 1);
                        print("Added name to scope: " + name);
                    }
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
            auto h = s->header(names);
            if (h != "/*no header*/")
            {
                print("Generated:");
                print(h + ";");
                output.push_back(h + ";\n");
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
        }
        return output;
    }

    string commaSep(vector<string>& items, string prefix, string suffix)
    {
        string line = "";
        for (int i =0; i < items.size(); i++)
        { 
            line += (prefix + items[i] + suffix);
            if (i+1 != items.size()) //If not on last iteration
            {
                line += ", ";
            }
        }
        return line;
    }

    string commaSep(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space, string prefix, string suffix)
    {
        string line = "";
        for (int i =0; i < symbols.size(); i++)
        { 
            line += (prefix + symbols[i]->source(names, n_space) + suffix);
            if (i+1 != symbols.size()) //If not on last iteration
            {
                line += ", ";
            }
        }
        return line;
    }

    string commaSepH(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space, string prefix, string suffix)
    {
        string line = "";
        for (int i =0; i < symbols.size(); i++)
        { 
            line += (prefix + symbols[i]->header(names, n_space) + suffix);
            if (i+1 != symbols.size()) //If not on last iteration
            {
                line += ", ";
            }
        }
        return line;
    }

    string buildArglist(vector<string>& arglist)
    {
        return commaSep(arglist, "auto ");
    }

    string templateArgList(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space)
    {
        string template_list = "";
        for (int i =0; i < symbols.size(); i++)
        { 
            auto repr = symbols[i]->source(names, n_space);
            template_list += ("T_" + repr + " " + repr);
            if (i+1 != symbols.size()) // If not on last iteration
            {
                template_list += ", ";
            }
        }
        return template_list;
    }

    string templateArgListH(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space)
    {
        string template_list = "";
        for (int i =0; i < symbols.size(); i++)
        { 
            auto repr = symbols[i]->header(names, n_space);
            template_list += ("T_" + repr + " " + repr);
            if (i+1 != template_list.size()) // If not on last iteration
            {
                template_list += ", ";
            }
        }
        return template_list;
    }

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
