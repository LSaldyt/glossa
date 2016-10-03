#include "Statements.hpp"
#include <iostream>

namespace Syntax
{

    std::string Statement::representation(){ return ""; }

    Expression::Expression() : base(std::make_shared<Symbol>(Integer(0))),
                               extensions(std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>()){}

    std::string Expression::representation()
    {
        std::string generated = "Expression: " + base->representation();
        for (auto e : extensions)
        {
            generated += (" " + std::get<0>(e)->representation() + " " + std::get<1>(e)->representation());
        }
        return generated;
    }

    Assignment::Assignment(std::vector<std::shared_ptr<Symbol>> construction_tokens)
    {
        identifier = construction_tokens[0]->representation();
        value      = construction_tokens[1];
    }

    Assignment::Assignment(){}

    std::string Assignment::representation()
    {
        return ("Assignment: (" + identifier + " = " + value->representation() + ")");
    }

    Function::Function() : body(std::vector<std::shared_ptr<Statement>>()){}

    std::string Function::representation()
    {
        std::string args = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            args += (argnames[i]);
            if (i+1 != argnames.size()) //If not on last iteration
            {
                args += ", ";
            }
        }

        std::string body_string = "";
        for (auto statement : body)
        {
            body_string += statement->representation();
        }

        return "Function " + identifier + ":\n\tArguments: (" + args + ")\n\tBody: " + body_string + "\n\tReturns: "+ return_expression->representation() + ")";
    }

    FunctionCall::FunctionCall(std::vector<std::shared_ptr<Symbol>> symbols) 
    {
        /*
        if (not symbols.empty())
        {
            identifier = symbols[0]->representation();
            args = std::vector<std::shared_ptr<Symbol>>(symbols.begin() + 1, symbols.end());
        }
        */
    }

    FunctionCall::FunctionCall() 
    {}

    std::string FunctionCall::representation()
    {
        std::string arglist = "";
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
