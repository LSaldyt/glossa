#include "expression.hpp"

namespace syntax
{
    Expression::Expression(){}
    Expression::Expression(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        auto symbols = symbol_groups[0];
        base = symbols[0];
        if (symbols.size() > 1)
        {
            if ((symbols.size() - 1) % 2 != 0)
            {
                throw named_exception("Cannot build expression extension from odd number of tokens");
            }
            for (int i = 1; i < symbols.size(); i += 2)
            {
                extensions.push_back(make_tuple(symbols[i], symbols[i + 1]));
            }
        }
    }

    string Expression::source(unordered_set<string>& names)
    {
        string generated = base->source(names);
        for (auto e : extensions)
        {
            generated += (" " + get<0>(e)->source(names) + " " + get<1>(e)->source(names));
        }
        return generated;
    }
}
