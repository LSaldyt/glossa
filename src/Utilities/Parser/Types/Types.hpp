#pragma once
#include <vector>
#include <string>
#include <functional>

namespace Parse
{
    using TokenComparator = std::function<bool(std::string)>;
    using Tokens          = std::vector<std::string>;
    using Token           = std::string;
}
