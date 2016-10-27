#include "functioncall.hpp"

namespace syntax
{
    FunctionCall::FunctionCall(vector<vector<shared_ptr<Symbol>>> symbol_groups) 
    {
        auto symbols = symbol_groups[0];
        if (not symbols.empty())
        {
            identifier = symbols[0]->name();
            args = slice(symbols, 1);
        }
    }

    FunctionCall::FunctionCall() 
    {}

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

    string FunctionCall::name()
    {
        return "none"; //identifier;
    }
}
