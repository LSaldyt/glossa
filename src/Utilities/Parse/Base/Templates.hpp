#pragma once
#include "Result.hpp"

namespace Parse
{
    using ParseFunction   = std::function<Result(Terms)>;
    using ParseFunctions  = std::vector<ParseFunction>;
    using Consumer        = std::function<Consumed(Terms)>;

    ParseFunction parseTemplate(Consumer consumer);
    ParseFunction singleTemplate(Comparator comparator);
    ParseFunction multiTemplate(Consumer consumer);
}
