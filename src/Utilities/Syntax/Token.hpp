#pragma once
#include <tuple>
#include <string>
#include "Symbols.hpp"
#include "../Parser/ParserBase/Types.hpp"

namespace Syntax
{
    using Token  = std::tuple<std::string, std::string>;
    using Tokens = std::vector<Token>;
}
