#pragma once
#include <vector>

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
    using Symbols = std::vector<Symbol>;

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
