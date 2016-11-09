#include "constructor.hpp"

namespace syntax
{
    Constructor::Constructor() 
    {
    }
    Constructor::Constructor(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        for (auto argname : symbol_groups[0])
        {
            argnames.push_back(argname->name());
        }
        body = symbol_groups[1];
    }
    string Constructor::source(unordered_set<string>& names, string n_space)
    {
        return "//Constructor";
        string arglist = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            arglist += ("auto " + argnames[i]);
            if (i+1 != argnames.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }
        string body_source = "";
        for (auto statement : generate(body))
        {
            body_source += statement;
        }

        string function_source = "";
        function_source += (n_space + "__init__" + "(" + arglist + ")");
        function_source += ("\n{\n" + body_source + "\n}");
        return function_source;
    }

    string Constructor::header(unordered_set<string>& names, string n_space)
    {
        return "//Constructor";
        string arglist = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            arglist += ("auto " + argnames[i]);
            if (i+1 != argnames.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }

        string function_source = "";
        function_source += (n_space + "__init__" + "(" + arglist + ")");
        return function_source;
    }
}
