#pragma once
#include "import.hpp"

namespace syntax
{
    struct Symbol
    {
        virtual string representation(){ return ""; }

        Symbol(){}
    };


    template < typename T>
    class Literal : public Symbol
    {
        T value;
    public:
        Literal(T set_value)
        {
            value = set_value; 
        }

        virtual string representation()
        {
            return "Literal: " + std::to_string(value);
        }
    };

    using SymbolGenerator  = function<shared_ptr<Symbol>(vector<string>)>;
    using SymbolGenerators = vector<SymbolGenerator>;

    struct Operator : public Symbol
    {
        string value;
        Operator(string set_value)
        {
            value = set_value;
        }
        string representation()
        {
            return "Operator: " + value;
        }
    };

    struct LogicalOperator : public Symbol
    {
        string value;
        LogicalOperator(string set_value)
        {
            value = set_value;
        }
        string representation()
        {
            return "LogicalOperator: " + value;
        }
    };

    struct Punctuator : public Symbol
    {
        string value;
        Punctuator(string set_value)
        {
            value = set_value;
        }
        string representation()
        {
            return "Punctuator: " + value;
        }
    };

    const auto opGenerator = [](string term){ return make_shared<Operator>(Operator(term));};
    const auto logicalOpGenerator = [](string term){return make_shared<LogicalOperator>(LogicalOperator(term));};
    const auto puncGenerator = [](string term){ return make_shared<Punctuator>(Punctuator(term));};

    using Integer = Literal<int>;
    const auto intGenerator = [](string s){ return make_shared<Integer>(Integer(stoi(s))); };

    struct Identifier : public Symbol
    {
        string name;
        Identifier(string set_name="")
        {
            name = set_name;
        }
        string representation()
        {
            return "Identifier: " + name;
        }
    };

    const auto identifierGenerator = [](string s){ return make_shared<Identifier>(Identifier(s)); };

    struct Keyword : public Symbol
    {
        string name;
        Keyword(string set_name)
        {
            name = set_name;
        }
        string representation()
        {
            return "Keyword: " + name;
        }
    };

    const auto keywordGenerator = [](string s){ return make_shared<Keyword>(Keyword(s)); };

    struct String : public Symbol
    {
        string value;
        String(string set_value)
        {
            value = set_value;
        }
        string representation()
        {
            return "String: \"" + value + "\"";
        }
    };

    const auto stringGenerator = [](string s){
        return make_shared<String>(String(string(s.begin() + 1, s.end() - 1)));
    };

    const auto single = [](function<shared_ptr<Symbol>(string)> f){
        return [f](vector<string> values){return f(values[0]); };
    };

    const unordered_map<string, SymbolGenerator> generatorMap = {
     {"literal",       single(intGenerator)},
     {"operator",   single(opGenerator)},
     {"identifier", single(identifierGenerator)},
     {"keyword",    single(keywordGenerator)},
     {"string",     single(stringGenerator)},
     {"punctuator", single(puncGenerator)},
     {"logicaloperator", single(logicalOpGenerator)}
    };

}
