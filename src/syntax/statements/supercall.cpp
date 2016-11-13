#include "supercall.hpp"

namespace syntax
{
    SuperCall::SuperCall(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        args = symbol_groups[0];
    }

    string SuperCall::source(unordered_set<string>& names, string n_space)
    {
        return "// supercall";
    }
}
