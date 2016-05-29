#include "Gen.hpp"

namespace Gen
{
    GenerationResult generate(const SymbolicStatementParsers& parsers, SymbolicTokens tokens)
    {
        return GenerationResult(false, "", tokens);
    }
}
