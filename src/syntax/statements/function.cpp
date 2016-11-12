#include "function.hpp"

namespace syntax
{
    Function::Function() 
    {
    }
    Function::Function(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        print(symbol_groups.size());
        identifier = symbol_groups[0][0]->name();
        for (auto argname : symbol_groups[1])
        {
            argnames.push_back(argname->name());
        }
        body = symbol_groups[2];
        is_void = symbol_groups[3].empty();
        if (not is_void)
        {
            return_expression = symbol_groups[3][0];
        }
    }
    string Function::name()
    {
        return "*" + identifier;
    }
    string Function::source(unordered_set<string>& names, string n_space)
    {
        string arglist = buildArglist(argnames);

        string body_source = "";
        for (auto statement : generate(body))
        {
            body_source += statement;
        }

        string return_source = "";
        if (not is_void)
        {
            return_source = return_expression->source(names);
        }

        string function_source = "";
        if (not is_void)
        {
            function_source += "auto ";
        }
        else
        {
            function_source += "void ";
        }
        function_source += (n_space + identifier + "(" + arglist + ")");
        if (not is_void)
        {
            function_source += (" -> decltype(" + return_source + ")");
        }
        function_source += ("\n{\n" + body_source);
        if (not is_void)
        {
            function_source += ("return " + return_source + ";");
        }
        function_source += "\n}";
        return function_source;
    }

    string Function::header(unordered_set<string>& names, string n_space)
    {
        string arglist = buildArglist(argnames);

        string return_source = "";
        if (not is_void)
        {
            return_source = return_expression->source(names);
        }

        string function_source = "";
        if (not is_void)
        {
            function_source += "auto ";
        }
        else
        {
            function_source += "void ";
        }
        function_source += (n_space + identifier + "(" + arglist + ")");
        if (not is_void)
        {
            function_source += (" -> decltype(" + return_source + ")");
        }
        return function_source;
    }
}
