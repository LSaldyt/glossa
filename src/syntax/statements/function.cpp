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
        args = symbol_groups[1];
        body = symbol_groups[2];
    }
    string Function::name()
    {
        return "*" + identifier;
    }
    string Function::source(unordered_set<string>& names, string n_space)
    {
        string function_source = "";
        if (identifier == "main")
        {
            function_source += "int main(int argc, char ** argv){\n";
            function_source += "std::vector<std::string> __args__; \n";
            function_source += "for (int i = 0; i < argc; i++)\n";
            function_source += "{__args__.push_back(argv[i]);}\n";
            for (auto statement : generate(body))
            {
                function_source += "    " + statement;
            }
            function_source += "}\n";
        }
        return function_source;
    }

    string Function::header(unordered_set<string>& names, string n_space)
    {
        if (identifier == "main")
        {
            return "// No main declaration required";
        }
        
        string template_line = "";
        if (not args.empty())
        {
            template_line = "template < " + commaSep(args, names, n_space, "typename T_") + " >\n";
        }
        string arglist = templateArgList(args, names, n_space);

        string body_source = "";
        for (auto statement : generate(body))
        {
            body_source += "    " + statement;
        }

        string function_source = template_line + " auto " + n_space + identifier + "(" + arglist + ")";
        function_source += "\n{\n" + body_source + "\n}";
        return function_source;
    }
}
