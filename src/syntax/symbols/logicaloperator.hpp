/// Copyright 2017 Lucas Saldyt
#pragma once
#include "../import.hpp"
#include "symbol.hpp"

namespace syntax
{
    /**
     * Abstract representation of an identifier
     */
    using LogicalOperator = StringLiteral;
    const auto logicalOpGenerator = [](string term){return make_shared<LogicalOperator>(LogicalOperator(term));};
}

