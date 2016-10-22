#pragma once
#include "../import.hpp"
#include "symbol.hpp"

namespace syntax
{
    using LogicalOperator = StringLiteral;
    const auto logicalOpGenerator = [](string term){return make_shared<LogicalOperator>(LogicalOperator(term));};
}

