#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

namespace Syntax
{
    struct Symbol
    {
        virtual std::string representation(){ return ""; }
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
            return std::to_string(value);
        }
    };

    using SymbolGenerator  = std::function<std::shared_ptr<Symbol>(std::string)>;
    using SymbolGenerators = std::vector<SymbolGenerator>;

    struct Operator : public Symbol
    {
        std::string value;
        Operator(std::string set_value)
        {
            value = set_value;
        }
    };

    const auto opGenerator = [](std::string term){ return std::make_shared<Operator>(Operator(term));};

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
            return name;
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
            return name;
        }
    };

    const auto keywordGenerator = [](std::string s){ return std::make_shared<Keyword>(Keyword(s)); };

    const std::unordered_map<std::string, SymbolGenerator> generatorMap = {
     {"type",       intGenerator},
     {"operator",   opGenerator},
     {"identifier", identifierGenerator},
     {"keyword",    keywordGenerator}
    };

}
