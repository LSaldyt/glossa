#include "assignment.hpp"

namespace syntax
{
    Assignment::Assignment(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        identifier   = symbol_groups[0][0]->name();
        value        = symbol_groups[1][0];
    }

    string Assignment::name()
    {
        return "*" + identifier;
    }
    string Assignment::source(unordered_set<string>& names, string n_space)
    {
        if (names.find(identifier) == names.end())
        {
            return "auto " + identifier + " = " + value->source(names);
        }
        else
        {
            return identifier + " = " + value->source(names);
        }
    }
}
