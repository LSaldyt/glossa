#include "conditional.hpp"

namespace syntax
{
    Conditional::Conditional(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        condition = symbol_groups[0][0];
        if_body   = symbol_groups[1];
        else_body = symbol_groups[2];
    }

    string Conditional::representation()
    {
        string if_body_rep   = "";
        string else_body_rep = "";
        for (auto s : if_body)
        {
            if_body_rep += s->representation();
        }
        for (auto s : else_body)
        {
            else_body_rep += s->representation();
        }
        return "Conditional:\nIf: (" + condition->representation() + ")\nThen: (" + if_body_rep + ")\nElse: (" + else_body_rep + ")";
    }
    string Conditional::source(unordered_set<string>& names)
    {
        auto if_body_source   = generate(if_body);
        auto else_body_source = generate(else_body);

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
