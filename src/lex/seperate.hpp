#pragma once
#include "import.hpp"

namespace lex
{
    using Seperator       = tuple<string, bool>;
    using Seperators      = vector<Seperator>;

    Terms seperate(const string& sentence, const Seperators &seperators, bool strings=true);
    Seperators readWhitespaceFile(string filename);

}
