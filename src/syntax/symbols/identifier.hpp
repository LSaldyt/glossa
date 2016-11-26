#pragma once
#include "literal.hpp"
namespace syntax
{
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value);
        string name();
        string representation(Generator& generator, unordered_set<string>& generated, string filetype);
        string source(Generator& generator, unordered_set<string>& generated, string n_space="");
        string header(Generator& generator, unordered_set<string>& generated, string n_space="");

    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
