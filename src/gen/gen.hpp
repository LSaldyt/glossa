#pragma once

#include "../syntax/statements.hpp"
#include "../tools/tools.hpp"

namespace gen
{
using namespace syntax;
using namespace tools;

vector<string> generate(vector<shared_ptr<Symbol>>& symbols);
}
