#include "memberaccess.hpp"

namespace syntax
{
    MemberAccess::MemberAccess(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        identifier   = symbol_groups[0][0]->name();
        value        = symbol_groups[1][0];
    }

    string MemberAccess::source(unordered_set<string>& names, string n_space)
    {
        return identifier + "." + value->source(names);
    }
}
