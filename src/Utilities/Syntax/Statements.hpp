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
            std::string template_line    = "";
            std::string declaration_line = "";
            // std::string return_exp       = expression.generator();
            std::string return_line      = "";

            // if(identifier == "main" && argnames.size() == 0)
            // {
            //     declaration_line = "int main()\n{\n";
            //     return_line = "    print(" + return_exp + ");\n    return 0;\n}";
            // }
            // else
            // {
            //     template_line = "template< ";
            //     for (auto argname : argnames)
            //     {
            //         template_line += ("typename T" + argname);
            //     }
            //     template_line += " >\n";
            //     declaration_line = "auto " + identifier + "(";
            //     if (argnames.size() > 0)
            //     {
            //         declaration_line += ("T" + argnames[0] + " " + argnames[0]);
            //     }
            //     for(unsigned i = 1; i < argnames.size(); i++)
            //     {
            //         declaration_line += (", T" + argnames[i] + " " + argnames[i]);
            //     }
            //     declaration_line += ") -> decltype(" + return_exp + ")\n{\n    ";
            //     return_line = "return " + return_exp + ";\n}\n";
            // }

            return template_line + declaration_line + return_line;
        }
    };
}
