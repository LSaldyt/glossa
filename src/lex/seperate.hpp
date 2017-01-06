#pragma once
#include "import.hpp"

namespace lex
{
    using Seperator = tuple<string, bool>;

    vector<string> seperate(const string& sentence, const vector<Seperator> &seperators, vector<char> strings={}, string inline_comment="");
    vector<Seperator> readWhitespaceFile(string filename);
}
