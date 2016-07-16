#include "Statements.hpp"

namespace Syntax
{

    std::string Statement::generator(){ return ""; }

    Expression::Expression() : base(std::make_shared<Symbol>(Integer(0))),
                   extensions(std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>())
    {
    }

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

    Function::Function() : body(std::make_shared<Statement>(Expression()))
    {

    }

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

        return "const auto " + identifier + " = [=](" + args + "){" + body->generator() + "};";
    }
}
