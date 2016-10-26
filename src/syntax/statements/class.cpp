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

string Class::source(unordered_set<string>& names)
{
    string representation = "";
    representation += "class ";
    representation += name;
    if (inheritance != "none")
    {
        representation += " : public ";
        representation += inheritance;
    }
    representation += "\n{\n";
    for (auto element : generate(body))
    {
        representation += element;
    }
    representation += "\n}";
    return representation;
}

}
