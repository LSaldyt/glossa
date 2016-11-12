#include "class.hpp"

namespace syntax
{

Class::Class(vector<vector<shared_ptr<Symbol>>> symbol_groups)
{
    name = symbol_groups[0][0]->name();
    if (not symbol_groups[1].empty())
    {
        inheritance = symbol_groups[1][0]->name();
    }
    else
    {
        inheritance = "none";
    }
    for (auto argname : symbol_groups[2])
    {
        constructor_args.push_back(argname->name());
    }
    constructor_body = symbol_groups[3];
    for (auto item : constructor_body)
    {
        auto name = item->name();
        if (name != "none")
        {
            if (std::string(name.begin(), name.begin() + 8) == "__self__")
            {
                members.push_back(std::string(name.begin() + 9, name.end()));
            }
        }
    } 
    body = symbol_groups[4];
}

string Class::header(unordered_set<string>& names, string n_space)
{
    string template_line = "template < ";
    for (auto member : members)
    {
        template_line += "typename T_" + member;
    } 
    template_line += " > ";

    string representation = "";
    representation += "class ";
    string mangled_name = "__" + name + "__";
    representation += mangled_name;
    if (inheritance != "none")
    {
        representation += " : public " + inheritance;
    }
    representation += "\n{\npublic:\n";
    for (auto member : members)
    {
        representation += "T_" + member + " " + member + ";\n"; // ex: T_name name
    } 
    for (auto element : generateHeader(constructor_body, names))
    {
        representation += element;
    }

    string inner_constructor_args = "";
    string initializers = "";
    for (int i =0; i < members.size(); i++)
    { 
        inner_constructor_args += ("T_" + members[i] + " set_" + members[i]);
        initializers += members[i] + "(set_" + members[i] + ")";
        if (i+1 != members.size()) // If not on last iteration
        {
            inner_constructor_args += ", ";
            initializers += ", ";
        }
    }

    representation += mangled_name + "(" + inner_constructor_args + ") : " + initializers + " {}\n";
    for (auto element : generate(body, names, n_space))
    {
        representation += element;
    }
    representation += "\n};\n";

    representation += "const auto " + name + " = [=](";
    representation += buildArglist(constructor_args);
    representation += "){\n";
    for (auto element : generate(constructor_body, names, n_space))
    {
        representation += element;
    }

    string deduction_line = "";
    string initialization_line = "";
    for (int i =0; i < members.size(); i++)
    { 
        deduction_line += ("decltype(" + members[i] + ")");
        initialization_line += members[i];
        if (i+1 != members.size()) // If not on last iteration
        {
            deduction_line += ", ";
            initialization_line += ", ";
        }
    }

    representation += "return " + mangled_name + "<" + deduction_line + ">" + "(" + initialization_line + ");";
    representation += "\n};\n";

    return template_line + representation;
}

string Class::source(unordered_set<string>& names, string n_space)
{
    return "";
}

}
