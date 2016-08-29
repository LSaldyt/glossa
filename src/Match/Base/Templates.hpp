#pragma once
#include "MatchResult.hpp"

namespace Match 
{
    using MatchFunction   = std::function<MatchResult(Terms)>;
    using MatchFunctions  = std::vector<MatchFunction>;
    using Consumer        = std::function<Consumed<Term>(Terms)>;

    MatchFunction matchTemplate(Consumer consumer);
    MatchFunction singleTemplate(Comparator comparator);
    MatchFunction multiTemplate(Consumer consumer);
}
