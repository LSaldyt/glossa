#include "conditional.hpp"

namespace syntax
{
    Conditional::Conditional(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        condition = symbol_groups[0][0];
        if_body   = symbol_groups[1];
        else_body = symbol_groups[2];
    }

    string Conditional::source(unordered_set<string>& names)
    {
        auto if_body_source   = generate(if_body, names);
        auto else_body_source = generate(else_body, names);

        string source = "if (" + condition->source(names) + "){ ";
        for (auto s : if_body_source)
        {
            source += s;
        }
        source += "} else { ";
        for (auto s : else_body_source)
        {
            source += s;
        }
        source += "}";
        return source;
    }
}
