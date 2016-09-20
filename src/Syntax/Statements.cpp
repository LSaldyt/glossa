#include "Statements.hpp"
#include <iostream>

namespace Syntax
{

    std::string Statement::representation(){ return ""; }

    Expression::Expression() : base(std::make_shared<Symbol>(Integer(0))),
                               extensions(std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>()){}


    std::string Expression::representation()
    {
        std::string generated = base->representation();
        for (auto e : extensions)
        {
            generated += (" " + std::get<0>(e)->representation() + " " + std::get<1>(e)->representation());
        }
        return generated;
    }

    Assignment::Assignment(std::vector<SymbolicTokens> construction_tokens)
    {
        identifier = construction_tokens[0][0].sub_type;

    }

    Assignment::Assignment()
    {

    }

    std::string Assignment::representation()
    {
        return ("const auto " + identifier + " = " + value.representation() + ";");
    }

    Function::Function() : body(std::vector<std::shared_ptr<Statement>>()){}

    std::string Function::representation()
    {
        std::string args = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            std::cout << "Argname: " << argnames[i] << std::endl;
            args += ("auto " + argnames[i]);
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

        return "const auto " + identifier + " = [=](" + args + "){" + body_string + " return "+ return_expression.representation() + "; };";
    }

    std::string FunctionCall::representation()
    {
        std::string arglist = "";
        for (int i =0; i < args.size(); i++)
        { 
            arglist += args[i];
            if (i+1 != args.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }
        return identifier + "(" + arglist + ");";
    }
}
