#pragma once
#include "literal.hpp"
namespace syntax
{
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value);
        virtual string name();
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype);
        virtual string abstract(int indent=0);
    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
