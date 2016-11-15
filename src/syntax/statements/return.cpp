#include "return.hpp"

namespace syntax
{
    Return::Return(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        value = symbol_groups[0][0];
    }

    string Return::source(unordered_set<string>& names, string n_space)
    {
        return "return " + value->source(names, n_space);
    }
}
