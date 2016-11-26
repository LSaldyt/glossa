#pragma once

// Avoid a circular include
namespace syntax
{
class Symbol;
}

#include "../tools/tools.hpp"
#include "generator.hpp"

namespace gen
{
using namespace syntax;
using namespace tools;

tuple<vector<string>, vector<string>> generateFiles(string filename, vector<shared_ptr<Symbol>>& symbols, Generator& generator);

vector<string> generate(vector<shared_ptr<Symbol>>& symbols, unordered_set<string> outer_names=unordered_set<string>(), string n_space="");
vector<string> generateHeader(vector<shared_ptr<Symbol>>& symbols, unordered_set<string> outer_names=unordered_set<string>());

string buildArglist(vector<string>& arglist);
string commaSep(vector<string>& items, string prefix="", string suffix="");
string commaSep(vector<shared_ptr<Symbol>>& items, unordered_set<string>& names, string n_space, string prefix="", string suffix="");
string commaSepH(vector<shared_ptr<Symbol>>& items, unordered_set<string>& names, string n_space, string prefix="", string suffix="");
string templateArgList(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space);
string templateArgListH(vector<shared_ptr<Symbol>>& symbols, unordered_set<string>& names, string n_space);

string sepWith(Generator& generator, const vector<shared_ptr<Symbol>>&, unordered_set<string>& names, string filetype, string sep=" ", string formatter="@");
string format(const string& inner, const string& formatter);
}
