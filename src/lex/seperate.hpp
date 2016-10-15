#pragma once
#include "import.hpp"

namespace lex
{
    using Seperator       = tuple<string, bool>;
    using Seperators      = vector<Seperator>;

    Terms seperate(const string& sentence, const Seperators &seperators);

    const Seperators whitespace =
    {
        make_tuple(" ",  false),
        make_tuple("\t", false),
        make_tuple("\n", false)
    };
}
