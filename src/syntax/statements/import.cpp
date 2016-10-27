#include "import.hpp"

namespace syntax
{
    Import::Import(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        filename = symbol_groups[0][0]->name();
    }

    string Import::source(unordered_set<string>& names, string n_space)
    {
        return "";
    }

    string Import::header(unordered_set<string>& names, string n_space)
    {
        return "#include \"" + filename + ".hpp\"\n";
    }
}
