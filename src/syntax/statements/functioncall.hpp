#pragma once
#include "statement.hpp"

namespace syntax
{
    struct FunctionCall : public Statement
    {
        FunctionCall();
        FunctionCall(vector<vector<shared_ptr<Symbol>>>);

        string              identifier;
        vector<shared_ptr<Symbol>> args;

        string              representation();
        string source(unordered_set<string>&);
        string name();
    };
}
