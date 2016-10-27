#include "whileloop.hpp"

namespace syntax
{
    WhileLoop::WhileLoop(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        condition = symbol_groups[0][0];
        body      = symbol_groups[1];
    }

    string WhileLoop::source(unordered_set<string>& names, string n_space)
    {
        string combined_body_source;
        auto body_source   = generate(body, names);
        for (auto& body_element : body_source)
        {
            combined_body_source += body_element;
        }
        return "while ( " + condition->source(names) + " ){ " + combined_body_source + " }";
    }
}
