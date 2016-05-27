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

    struct Statement {};

    struct Symbol {};

    using SymbolGenerator  = std::function<std::shared_ptr<Symbol>(std::string)>;
    using SymbolGenerators = std::vector<SymbolGenerator>;

    struct Operator : public Symbol
    {
        Op op;
        Operator(Op set_op){op = set_op;}
    };

    const SymbolGenerator opGenerator = [](std::string term){ return std::make_shared<Operator>(Operator(Op(term[0])));};

    struct Expression : public Symbol {};
    struct BinaryExpression : public Expression
    {
        Operator    op;
        Expression* e1;
        Expression* e2;
    };

    struct Literal : public Expression
    {

    };

    struct Integer : public Literal
    {
        int value;
        Integer(int set_value) { value = set_value; }
    };

    const SymbolGenerator intGenerator = [](std::string s){ return std::make_shared<Integer>(Integer(std::stoi(s))); };

    const std::unordered_map<std::string, SymbolGenerator> generatorMap = {
     {"number", intGenerator},
     {"operator",  opGenerator}
    };
}
