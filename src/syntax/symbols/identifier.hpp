#pragma once
#include "literal.hpp"
namespace syntax
{
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value);
        string name();
    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
