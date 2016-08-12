#pragma once
#include "Import.hpp"

namespace Lex
{
    using Seperator       = std::tuple<std::string, bool>;
    using Seperators      = std::vector<Seperator>;

    Terms seperate(const std::string& sentence, const Seperators &seperators);

    const Seperators whitespace =
    {
        std::make_tuple(" ",  false),
        std::make_tuple("\t", false),
        std::make_tuple("\n", false)
    };
}
