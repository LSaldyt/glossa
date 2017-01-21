#pragma once
#include "identifier.hpp"
#include "literal.hpp"
#include "string.hpp"
#include "operator.hpp"
#include "logicaloperator.hpp"
#include "multisymbol.hpp"

namespace syntax
{
    using Punctuator = StringLiteral;
    using Integer    = Literal<int>;
    using Keyword    = StringLiteral;
    using Tab        = StringLiteral;
    using Newline    = StringLiteral;
    using Comment    = StringLiteral;

    const auto stringGenerator = [](string s){
        if (s.size() >= 2)
        {
            return make_shared<String>(String(string(s.begin() + 1, s.end() - 1)));
        }
        else
        {
            print("WARNING: Strange string (" + s + ")");
            return make_shared<String>(String(s));
        }
    };
    const auto keywordGenerator = [](string s){ return make_shared<Keyword>(Keyword(s)); };
    const auto intGenerator     = [](string s){ return make_shared<Integer>(Integer(stoi(s))); };
    const auto puncGenerator    = [](string s){ return make_shared<Punctuator>(Punctuator(s));};
    const auto tabGenerator     = [](string s){ return make_shared<Tab>(Tab(s));};
    const auto commentGenerator = [](string s){ return make_shared<Comment>(Comment(string(s.begin() + 1, s.end()))); };

    const auto literalGenerator = [](string s)
    {
        shared_ptr<Symbol> value;
        try {
            value = intGenerator(s);
        }
        catch(std::exception)
        {
            value = stringGenerator(s);
        }
        return value;
    };


    const auto single = [](function<shared_ptr<Symbol>(string)> f){
        return [f](vector<string> values){return f(values[0]); };
    };
    const unordered_map<string, SymbolGenerator> generatorMap = {
     {"literal",       single(literalGenerator)},
     {"operator",   single(opGenerator)},
     {"identifier", single(identifierGenerator)},
     {"keyword",    single(keywordGenerator)},
     {"punctuator", single(puncGenerator)},
     {"logicaloperator", single(logicalOpGenerator)},
     {"tab",             single(tabGenerator)},
     {"comment", single(commentGenerator)}
    };
}
