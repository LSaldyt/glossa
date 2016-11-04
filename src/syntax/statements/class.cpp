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
    body = symbol_groups[2];
}

string Class::header(unordered_set<string>& names, string n_space)
{
    string representation = "";
    representation += "class ";
    representation += name;
    if (inheritance != "none")
    {
        representation += " : public ";
        representation += inheritance;
    }
    representation += "\n{\npublic:\n";
    for (auto element : generateHeader(body, names))
    {
        representation += element;
    }
    representation += "\n}";
    return representation;
}

string Class::source(unordered_set<string>& names, string n_space)
{
    string representation = "";
    for (auto element : generate(body, names, name + "::"))
    {
        representation += element;
    }
    size_t start_pos = 0;
    while((start_pos = representation.find("__init__", start_pos)) != std::string::npos) 
    {
        representation.replace(start_pos, 8, name); // Replace __init__ function with classname
        start_pos += name.length(); 
    }
    return representation;
}

}
