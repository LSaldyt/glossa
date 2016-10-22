#include "vector.hpp"

namespace syntax
{

Vector::Vector(vector<vector<shared_ptr<Symbol>>> symbol_groups)
{
    content = symbol_groups[0];
}

string Vector::representation()
{
    string content_representation = "";
    for (int i =0; i < content.size(); i++)
    { 
        content_representation = content[i]->representation();
        if (i+1 != content.size()) //If not on last iteration
        {
            content_representation += ", \n";
        }
    }
    return "Vector ( " + content_representation + " )";
}

string Vector::source(unordered_set<string>& names)
{
    string content_initializer = "";
    for (int i =0; i < content.size(); i++)
    { 
        content_initializer += content[i]->source(names);
        if (i+1 != content.size()) //If not on last iteration
        {
            content_initializer += ", ";
        }
    }

    return "std::vector<decltype(" + content[0]->source(names) + ")>({" + content_initializer + "});";
}

}
