#pragma once
#include "import.hpp"

namespace lex
{
    using Seperator       = tuple<string, bool>;
    using Seperators      = vector<Seperator>;

    vector<string> seperate(const string& sentence, const Seperators &seperators, vector<char> strings={}, string inline_comment="");
    Seperators readWhitespaceFile(string filename);

}
