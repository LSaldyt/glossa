#include "statements.hpp"

namespace syntax
{

    string Statement::representation(){ return ""; }

    Expression::Expression(){}
    Expression::Expression(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        auto symbols = symbol_groups[0];
        base = symbols[0];
        if (symbols.size() > 1)
        {
            if ((symbols.size() - 1) % 2 != 0)
            {
                throw named_exception("Cannot build expression extension from odd number of tokens");
            }
            for (int i = 1; i < symbols.size(); i += 2)
            {
                extensions.push_back(make_tuple(symbols[i], symbols[i + 1]));
            }
        }
    }

    string Expression::representation()
    {
        string generated = "Expression: " + base->representation();
        for (auto e : extensions)
        {
            generated += (" " + get<0>(e)->representation() + " " + get<1>(e)->representation());
        }
        return generated;
    }
    string Expression::source(unordered_set<string>& names)
    {
        string generated = base->source(names);
        for (auto e : extensions)
        {
            generated += (" " + get<0>(e)->source(names) + " " + get<1>(e)->source(names));
        }
        return generated;
    }

    Assignment::Assignment(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        auto symbols = symbol_groups[0];
        identifier   = symbols[0]->representation();
        value        = symbols[1];
    }

    string Assignment::representation()
    {
        return ("Assignment: (" + identifier + " = " + value->representation() + ")");
    }
    string Assignment::name()
    {
        return identifier;
    }
    string Assignment::source(unordered_set<string>& names)
    {
        if (names.find(identifier) == names.end())
        {
            return "auto " + identifier + " = " + value->source(names);
        }
        else
        {
            return identifier + " = " + value->source(names);
        }
    }

    Function::Function() 
    {
    }
    Function::Function(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        print(symbol_groups.size());
        identifier = symbol_groups[0][0]->representation();
        for (auto argname : symbol_groups[1])
        {
            argnames.push_back(argname->representation());
        }
        body = symbol_groups[2];
        return_expression = symbol_groups[3][0];
    }

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

        return "Function " + identifier + ":\n\tArguments: (" + args + ")\n\tBody: " + body_string + "\n\tReturns: " + return_expression->representation() + ")";
    }
    string Function::name()
    {
        return identifier;
    }
    string Function::source(unordered_set<string>& names)
    {
        string arglist = "";
        for (int i =0; i < argnames.size(); i++)
        { 
            arglist += ("auto " + argnames[i]);
            if (i+1 != argnames.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }
        string body_source = "";
        for (auto statement : body)
        {
            body_source += statement->source(names);
        }

        return "auto " + identifier + " = [=](" + arglist + "){" + body_source + "return " + return_expression->source(names) + "; }";
    }

    FunctionCall::FunctionCall(vector<vector<shared_ptr<Symbol>>> symbol_groups) 
    {
        auto symbols = symbol_groups[0];
        identifier = symbols[0]->representation();
        if (not symbols.empty())
        {
            args = slice(symbols, 1);
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
    string FunctionCall::source(unordered_set<string>& names)
    {
        string arglist = "";
        for (int i =0; i < args.size(); i++)
        { 
            arglist += args[i]->source(names);
            if (i+1 != args.size()) //If not on last iteration
            {
                arglist += ", ";
            }
        }
        return identifier + "(" + arglist + ")";
    }

    Conditional::Conditional(vector<vector<shared_ptr<Symbol>>> symbol_groups)
    {
        condition = symbol_groups[0][0];
        if_body   = symbol_groups[1];
        else_body = symbol_groups[2];
    }

    string Conditional::representation()
    {
        string if_body_rep   = "";
        string else_body_rep = "";
        for (auto s : if_body)
        {
            if_body_rep += s->representation();
        }
        for (auto s : else_body)
        {
            else_body_rep += s->representation();
        }
        return "Conditional:\nIf: (" + condition->representation() + ")\nThen: (" + if_body_rep + ")\nElse: (" + else_body_rep + ")";
    }
    string Conditional::source(unordered_set<string>& names)
    {
        return "if (" + condition->source(names) + "){} else{}";
    }
}
