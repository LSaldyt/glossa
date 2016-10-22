#pragma once
#include "literal.hpp"
namespace syntax
{
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value) : StringLiteral(set_value){}
        string name(){return value;}
    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
