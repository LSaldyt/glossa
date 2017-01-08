#pragma once
#include "literal.hpp"
namespace syntax
{
    /**
     * Abstract representation of an identifier
     */
    struct Identifier : public StringLiteral
    {
        Identifier(string set_value);
        virtual string name();
        virtual string representation(Generator& generator, unordered_set<string>& generated, string filetype, int nesting=0);
        virtual string abstract(int indent=0);
    };
    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };
}
