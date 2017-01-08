#pragma once
#include "import.hpp"
#include "consumed.hpp"
#include "../../types/result.hpp"

namespace match 
{
    /// Single specialization of Result type (string)
    using MatchResult = Result<string>; 
}
