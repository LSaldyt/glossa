#include "assignment.hpp"

namespace syntax
{
    Assignment::Assignment(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        auto symbols = symbol_groups[0];
        identifier   = symbols[0]->name();
        value        = symbols[1];
    }

    string Assignment::representation()
    {
        return ("Assignment: (" + identifier + " = " + value->representation() + ")");
    }
    string Assignment::name()
    {
        return identifier;
    }
    string Assignment::source(unordered_set<string>& names)
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
