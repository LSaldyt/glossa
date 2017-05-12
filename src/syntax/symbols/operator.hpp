/// Copyright 2017 Lucas Saldyt
#pragma once
#include "../import.hpp"
namespace syntax
{
    using Operator = StringLiteral;
    const auto opGenerator = [](string term){ return make_shared<Operator>(Operator(term));};
}
