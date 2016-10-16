#pragma once

// Avoid a circular include
namespace syntax
{
class Symbol;
}

#include "../tools/tools.hpp"

namespace gen
{
using namespace syntax;
using namespace tools;

vector<string> generate(vector<shared_ptr<Symbol>>& symbols);
}
