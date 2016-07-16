#pragma once
#include "Symbols.hpp"
#include "Token.hpp"
namespace Syntax
{

    struct Statement
    {
        virtual std::string generator(){ return ""; }
    };

    using StatementGenerator = std::function<std::shared_ptr<Statement>(SymbolicTokens)>;

    struct Expression : public Statement
    {
        std::shared_ptr<Symbol> base;
        std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>> extensions;
        Expression() : base(std::make_shared<Symbol>(Integer(0))),
                       extensions(std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>())
        {
        }

        std::string generator()
        {
            std::string generated = base->representation();
            for (auto e : extensions)
            {
                generated += (" " + std::get<0>(e)->representation() + " " + std::get<1>(e)->representation());
            }
            return generated;
        }
    };

    struct Assignment : public Statement
    {
        std::string identifier;
        Expression value;

        std::string generator()
        {
            return ("const auto " + identifier + " = " + value.generator() + ";");
        }
    };

    struct Function : public Statement
    {
        std::string                identifier;
        std::vector<std::string>   argnames;
        std::shared_ptr<Statement> body;

        Function() : body(std::make_shared<Statement>(Expression()))
        {

        }

        std::string generator()
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
  
            return "const auto " + identifier + " [=](" + args + "){" + body->generator() + "}";
        }
    };
}
