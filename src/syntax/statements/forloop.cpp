#include "forloop.hpp"

namespace syntax
{
    ForLoop::ForLoop(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        loop_variable = symbol_groups[0][0]->name();
        loop_source   = symbol_groups[1][0];
        body          = symbol_groups[2];
        print("FOR LOOP SIZE");
        print(body.size());
    }

    string ForLoop::representation()
    {
        return "forloop";

    }
    string ForLoop::source(unordered_set<string>& names)
    {
        return "for (auto& " + loop_variable + " : " + loop_source->source(names) + "){}";
    }
}
