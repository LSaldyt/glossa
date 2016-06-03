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
            if (tokens.begin() != tokens.end())
            {
                base = tokens.begin()->value;
            }
            for (auto it = tokens.begin() + 1; it != tokens.end(); ++it)
            {
                if(it+1 != tokens.end())
                {
                    extensions.push_back(std::make_tuple(it->value, (it+1)->value));
                }
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
            return (type + " " + identifier + " = " + value.generator() + ";");
        }
    };

    struct Function : public Statement
    {
        std::string identifier;

        std::string generator()
        {
            return "int f(int x);";
        }
    };

    const StatementGenerator FunctionGenerator = [](SymbolicTokens tokens)
    {
        return std::make_shared<Function>(Function());
    };

    const StatementGenerator AssignmentGenerator = [](SymbolicTokens tokens)
    {
        auto identifier = tokens[0].value->representation();

        auto type       = tokens[2].sub_type;
        SymbolicTokens expression_tokens;
        if (tokens.size() >= 3)
        {
            expression_tokens = SymbolicTokens(tokens.begin() + 2, tokens.end());
        }
        return std::make_shared<Assignment>(Assignment(identifier, Expression(expression_tokens), type));
    };

}
