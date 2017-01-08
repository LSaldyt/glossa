#pragma once

// Avoid a circular include
namespace syntax
{
class Symbol;
}

#include "../tools/tools.hpp"
#include "generator.hpp"

/**
 * Definitions for user defined code generation in any given programming language
 */
namespace gen
{
using namespace syntax;
using namespace tools;

tuple<vector<string>, vector<string>> generateFiles(string filename, vector<shared_ptr<Symbol>>& symbols, Generator& generator);

string sepWith(Generator& generator, const vector<shared_ptr<Symbol>>&, unordered_set<string>& names, string filetype, string sep=" ", string formatter="@", int nesting=0);
string format(const string& inner, const string& formatter);
}
