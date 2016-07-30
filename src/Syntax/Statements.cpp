#include "Statements.hpp"
#include <iostream>

namespace Syntax
{

    std::string Statement::generator(){ return ""; }

    Expression::Expression() : base(std::make_shared<Symbol>(Integer(0))),
                               extensions(std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>()){}

    std::string Expression::generator()
    {
        std::string generated = base->representation();
        for (auto e : extensions)
        {
            generated += (" " + std::get<0>(e)->representation() + " " + std::get<1>(e)->representation());
        }
        return generated;
    }

    std::string Assignment::generator()
    {
        return ("const auto " + identifier + " = " + value.generator() + ";");
    }

    Function::Function() : body(std::vector<std::shared_ptr<Statement>>()){}

    std::string Function::generator()
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
            body_string += statement->generator();
        }

        return "const auto " + identifier + " = [=](" + args + "){" + body_string + " return "+ return_expression.generator() + "; };";
    }

    std::string FunctionCall::generator()
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
