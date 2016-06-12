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
        Expression(std::shared_ptr<Symbol> set_base,
                   std::vector<std::tuple<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>> set_extensions)
        {
            base = set_base;
            extensions = set_extensions;
        }

        Expression(SymbolicTokens tokens)
        {
            auto it = tokens.begin();
            if (it != tokens.end())
            {
                base = it->value;
            }
            it++;
            while(it != tokens.end())
            {
                if(it + 1 != tokens.end())
                {
                    extensions.push_back(std::make_tuple(it->value, (it+1)->value));
                    it++;
                }
                else
                {
                    break;
                }
                it++;
            }
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
        std::string type;
        Assignment(std::string set_identifier,
                   Expression set_value,
                   std::string set_type) : value(set_value)
        {
            identifier = set_identifier;
            type       = set_type;
        }

        std::string generator()
        {
            return ("const " + type + " " + identifier + " = " + value.generator() + ";");
        }
    };

    struct Function : public Statement
    {
        std::string identifier;
        std::vector<std::string> argnames;
        Expression expression;

        Function(std::string set_identifier,
                 std::vector<std::string> set_argnames,
                 Expression set_expression) : expression(set_expression)
        {
            identifier = set_identifier;
            argnames   = set_argnames;
        }

        std::string generator()
        {
            std::string template_line    = "";
            std::string declaration_line = "";
            std::string return_exp       = expression.generator();
            std::string return_line      = "";

            if(identifier == "main" && argnames.size() == 0)
            {
                declaration_line = "int main()\n{\n";
                return_line = "    print(" + return_exp + ");\n    return 0;\n}";
            }
            else
            {
                template_line = "template< ";
                for (auto argname : argnames)
                {
                    template_line += ("typename T" + argname);
                }
                template_line += " >\n";
                declaration_line = "auto " + identifier + "(";
                if (argnames.size() > 0)
                {
                    declaration_line += ("T" + argnames[0] + " " + argnames[0]);
                }
                for(unsigned i = 1; i < argnames.size(); i++)
                {
                    declaration_line += (", T" + argnames[i] + " " + argnames[i]);
                }
                declaration_line += ") -> decltype(" + return_exp + ")\n{\n    ";
                return_line = "return " + return_exp + ";\n}\n";
            }

            return template_line + declaration_line + return_line;
        }
    };

    const StatementGenerator FunctionGenerator = [](SymbolicTokens tokens)
    {
        auto identifier = tokens[0].value->representation();
        std::vector<std::string> argnames;
        auto it = tokens.begin() + 2;
        while(it->value->representation() != ")")
        {
            argnames.push_back(it->value->representation());
            it++;
        }

        it += 4;
        auto return_expression = Expression(SymbolicTokens(it, tokens.end()));

        return std::make_shared<Function>(Function(identifier, argnames, return_expression));
    };

    const StatementGenerator AssignmentGenerator = [](SymbolicTokens tokens)
    {
        auto identifier = tokens[0].value->representation();

        auto type       = tokens[2].sub_type;
        SymbolicTokens expression_tokens;
        if (tokens.size() >= 3)
        {
            expression_tokens = SymbolicTokens(tokens.begin() + 2, tokens.end() - 1);
        }
        return std::make_shared<Assignment>(Assignment(identifier, Expression(expression_tokens), type));
    };

}
