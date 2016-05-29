#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

namespace Syntax
{
    enum class Op : char
    {
        Add = '+',
        Sub = '-',
        Mul = '*',
        Div = '/',
        Exp = '^'
    };

    struct Symbol
    {
        virtual std::string representation(){ return ""; }
    };

    using SymbolGenerator  = std::function<std::shared_ptr<Symbol>(std::string)>;
    using SymbolGenerators = std::vector<SymbolGenerator>;

    struct Operator : public Symbol
    {
        Op op;
        Operator(Op set_op){op = set_op;}
        std::string representation()
        {
            return "";
        }
    };

    const auto opGenerator = [](std::string term){ return std::make_shared<Operator>(Operator(Op(term[0])));};

    struct Expression : public Symbol {};

    struct Literal : public Expression
    {

    };

    struct Integer : public Literal
    {
        int value;
        Integer(int set_value) { value = set_value; }
        std::string representation()
        {
            return std::to_string(value);
        }
    };

    const auto intGenerator = [](std::string s){ return std::make_shared<Integer>(Integer(std::stoi(s))); };

    struct Identifier : public Symbol
    {
        std::string name;
        Identifier(std::string set_name)
        {
            name = set_name;
        }
        std::string representation()
        {
            return name;
        }
    };

    const auto identifierGenerator = [](std::string s){ return std::make_shared<Identifier>(Identifier(s)); };

    const std::unordered_map<std::string, SymbolGenerator> generatorMap = {
     {"int",        intGenerator},
     {"operator",   opGenerator},
     {"identifier", identifierGenerator}
    };

}
