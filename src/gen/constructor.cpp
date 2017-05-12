/// Copyright 2017 Lucas Saldyt
#include "constructor.hpp"

namespace gen
{
void addNewLine(vector<string>& generated)
{
    generated.push_back("\n");
}
void addNewLine(vector<vector<string>>& generated)
{}
void addNewLine(vector<shared_ptr<Symbol>>& generated)
{}
}
