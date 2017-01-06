#pragma once
#include "import.hpp"

namespace lex
{
    using Seperator       = tuple<vector<string>, bool>;
    using Seperators      = vector<Seperator>;

    vector<string> seperate(const vector<string>& sentence, const Seperators &seperators, vector<char> vector<string>={}, vector<string> inline_comment="");
    Seperators readWhitespaceFile(vector<string> filename);

}
