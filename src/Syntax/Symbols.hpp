#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

namespace Syntax
{
    struct Symbol
    {
        virtual std::string representation(){ return ""; }

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

        virtual std::string representation()
        {
            return "Literal: " + std::to_string(value);
        }
    };

    using SymbolGenerator  = std::function<std::shared_ptr<Symbol>(std::vector<std::string>)>;
    using SymbolGenerators = std::vector<SymbolGenerator>;

    struct Operator : public Symbol
    {
        std::string value;
        Operator(std::string set_value)
        {
            value = set_value;
        }
        std::string representation()
        {
            return "Operator: " + value;
        }
    };

    struct Punctuator : public Symbol
    {
        std::string value;
        Punctuator(std::string set_value)
        {
            value = set_value;
        }
        std::string representation()
        {
            return "Punctuator: " + value;
        }
    };

    const auto opGenerator = [](std::string term){ return std::make_shared<Operator>(Operator(term));};
    const auto puncGenerator = [](std::string term){ return std::make_shared<Punctuator>(Punctuator(term));};

    using Integer = Literal<int>;
    const auto intGenerator = [](std::string s){ return std::make_shared<Integer>(Integer(std::stoi(s))); };

    struct Identifier : public Symbol
    {
        std::string name;
        Identifier(std::string set_name="")
        {
            name = set_name;
        }
        std::string representation()
        {
            return "Identifier: " + name;
        }
    };

    const auto identifierGenerator = [](std::string s){ return std::make_shared<Identifier>(Identifier(s)); };

    struct Keyword : public Symbol
    {
        std::string name;
        Keyword(std::string set_name)
        {
            name = set_name;
        }
        std::string representation()
        {
            return "Keyword: " + name;
        }
    };

    const auto keywordGenerator = [](std::string s){ return std::make_shared<Keyword>(Keyword(s)); };

    struct String : public Symbol
    {
        std::string value;
        String(std::string set_value)
        {
            value = set_value;
        }
        std::string representation()
        {
            return "String: \"" + value + "\"";
        }
    };

    const auto stringGenerator = [](std::string s){
        return std::make_shared<String>(String(std::string(s.begin() + 1, s.end() - 1)));
    };

    const auto single = [](std::function<std::shared_ptr<Symbol>(std::string)> f){
        return [f](std::vector<std::string> values){return f(values[0]); };
    };

    const std::unordered_map<std::string, SymbolGenerator> generatorMap = {
     {"literal",       single(intGenerator)},
     {"operator",   single(opGenerator)},
     {"identifier", single(identifierGenerator)},
     {"keyword",    single(keywordGenerator)},
     {"string",     single(stringGenerator)},
     {"punctuator", single(puncGenerator)}
    };

}
