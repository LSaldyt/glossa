#include "memberinit.hpp"

namespace syntax
{
    MemberInit::MemberInit(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        identifier   = symbol_groups[0][0]->name();
        value        = symbol_groups[1][0];
    }

    string MemberInit::source(unordered_set<string>& names, string n_space)
    {
        return identifier + " = " + value->source(names, n_space);
    }
    string MemberInit::header(unordered_set<string>& names, string n_space)
    {
        string val_source = value->source(names, n_space);
        return "decltype(" + val_source + ") " +  identifier + " = " + val_source;
    }
}
