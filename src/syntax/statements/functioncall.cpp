#include "functioncall.hpp"

namespace syntax
{
    FunctionCall::FunctionCall(vector<vector<shared_ptr<Symbol>>> symbol_groups) 
    {
        auto symbols = symbol_groups[0];
        identifier = symbols[0]->name();
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
}
