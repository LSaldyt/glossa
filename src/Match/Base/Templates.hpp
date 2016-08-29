#pragma once
#include "Result.hpp"

namespace Match 
{
    using MatchFunction   = std::function<Result(Terms)>;
    using MatchFunctions  = std::vector<MatchFunction>;
    using Consumer        = std::function<Consumed<Term>(Terms)>;

    MatchFunction matchTemplate(Consumer consumer);
    MatchFunction singleTemplate(Comparator comparator);
    MatchFunction multiTemplate(Consumer consumer);
}
