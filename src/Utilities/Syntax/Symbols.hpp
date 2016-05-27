#pragma once
#include <vector>
#include <memory>

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

    struct Expression : public Symbol {};

    struct Operator : public Symbol
    {
        Op op;
        Operator(Op set_op){op = set_op;}
    };

    const SymbolGenerator opGenerator = [](std::string term){ return std::make_shared<Operator>(Operator(Op(term[0])));};

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
}
