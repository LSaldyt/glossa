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
    if (not symbol_groups[3].empty())
    {
        super_call = symbol_groups[3][0];
    }
    constructor_body = symbol_groups[4];
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
    body = symbol_groups[5];
}

string Class::header(unordered_set<string>& names, string n_space)
{

    // Treat inheritance as member, but don't declare it in the same way as other members
    vector<string> templatized_types = members;
    if (inheritance != "none")
    {
        templatized_types.push_back("inheritance");
    }

    string template_line = "template < " + commaSep(templatized_types, "typename T_") + " >\n";

    string representation = "class ";
    string mangled_name = "__" + name + "__";
    representation += mangled_name;
    if (inheritance != "none")
    {
        representation += " : public T_inheritance";
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

    if (inheritance != "none")
    {
        if (initializers.size() > 0)
        {
            initializers += ", ";
        }
        if (inner_constructor_args.size() > 0)
        {
            inner_constructor_args += ", ";
        }
        initializers += "T_inheritance(set_inheritance)"; 
        inner_constructor_args += "auto set_inheritance";
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

    string deduction_line      = commaSep(members, "decltype(", ")");
    string initialization_line = commaSep(members);

    if (inheritance != "none")
    {
        representation += "auto set_inheritance = " + inheritance + "(" + super_call->source(names, n_space) + ");\n";
        if (deduction_line.size() > 0)
        {
            deduction_line += ", ";
        }
        deduction_line += "decltype(set_inheritance)";
        if (initialization_line.size() > 0)
        {
            initialization_line += ", ";
        }
        initialization_line += "set_inheritance";

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
