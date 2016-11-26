#pragma once
#include "literal.hpp"
namespace syntax
{
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value);
        string name();
        string representation(Generator& generator, unordered_set<string>& generated, string filetype);
    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
