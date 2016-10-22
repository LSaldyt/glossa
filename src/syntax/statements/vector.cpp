#include "vector.hpp"

namespace syntax
{

Vector::Vector(vector<vector<shared_ptr<Symbol>>> symbol_groups)
{
    content = symbol_groups[0];
}

string Vector::source(unordered_set<string>& names)
{
    return "std::vector<int>();";
}

}
