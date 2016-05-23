#pragma once
enum Operator
{
    Add,
    Sub,
    Mul,
    Div,
    Exp
};

struct Expression {};

struct Integer : public Expression
{
    int value;
};

struct BinaryExpression : public Expression
{
    Operator    op;
    Expression* e1;
    Expression* e2;
};

struct Parens : public Expression
{
    Expression* e;
};
