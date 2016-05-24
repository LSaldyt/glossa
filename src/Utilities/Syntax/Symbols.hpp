#pragma once
#include <vector>
#include <memory>

namespace Syntax
{
    enum Op
    {
        Add,
        Sub,
        Mul,
        Div,
        Exp
    };

    struct Statement {};

    struct Symbol {};

    using SymbolGenerator  = std::function<Symbol(void)>;
    using SymbolGenerators = std::vector<SymbolGenerator>;

    struct Expression : public Symbol {};

    struct Operator : public Symbol
    {
        Op op;
    };

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
    };
}
