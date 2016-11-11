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
        return ("auto __new_members = std::tuple_cat(__members, std::make_tuple('a'));\n__access_map[" + identifier + "] = 0;");
        //return "// " + identifier;
    }
}
