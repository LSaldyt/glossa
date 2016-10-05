#include "Statements.hpp"
#include <iostream>

namespace Syntax
{

    string Statement::representation(){ return ""; }

    Expression::Expression() : base(make_shared<Symbol>(Integer(0))),
                               extensions(vector<tuple<shared_ptr<Symbol>, shared_ptr<Symbol>>>()){}

    string Expression::representation()
    {
        string generated = "Expression: " + base->representation();
        for (auto e : extensions)
        {
            generated += (" " + get<0>(e)->representation() + " " + get<1>(e)->representation());
        }
        return generated;
    }

    Assignment::Assignment(vector<shared_ptr<Symbol>> construction_tokens)
    {
        identifier = construction_tokens[0]->representation();
        value      = construction_tokens[1];
    }

    Assignment::Assignment(){}

    string Assignment::representation()
    {
        return ("Assignment: (" + identifier + " = " + value->representation() + ")");
    }

    Function::Function() : body(vector<shared_ptr<Statement>>()){}

    string Function::representation()
    {
        string args = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            args += (argnames[i]);
            if (i+1 != argnames.size()) //If not on last iteration
            {
                args += ", ";
            }
        }

        string body_string = "";
        for (auto statement : body)
        {
            body_string += statement->representation();
        }

        return "Function " + identifier + ":\n\tArguments: (" + args + ")\n\tBody: " + body_string + "\n\tReturns: "+ return_expression->representation() + ")";
    }

    FunctionCall::FunctionCall(vector<shared_ptr<Symbol>> symbols) 
    {
        identifier = symbols[0]->representation();
        if (not symbols.empty())
        {
            args = vector<shared_ptr<Symbol>>(symbols.begin() + 1, symbols.end());
        }
    }

    FunctionCall::FunctionCall() 
    {}

    string FunctionCall::representation()
    {
        string arglist = "";
        for (int i =0; i < args.size(); i++)
        { 
            arglist += args[i]->representation();
            if (i+1 != args.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }
        return "FunctionCall " + identifier + ": (\n\tArguments: (" + arglist + "))";
    }
}
