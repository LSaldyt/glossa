#include "forloop.hpp"

namespace syntax
{
    ForLoop::ForLoop(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        loop_variable = symbol_groups[0][0]->name();
        loop_source   = symbol_groups[1][0];
        body          = symbol_groups[2];
    }

    string ForLoop::source(unordered_set<string>& names)
    {
        string combined_body_source;
        auto body_source   = generate(body);
        for (auto& body_element : body_source)
        {
            combined_body_source += body_element;
        }
        return "for (auto& " + loop_variable + " : " + loop_source->source(names) + "){ " + combined_body_source + " }";
    }
}
