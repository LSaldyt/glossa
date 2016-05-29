#pragma once
#include "Import.hpp"
#include "GenResult.hpp"

namespace Gen
{
    GenerationResult generate(const SymbolicStatementParsers&, SymbolicTokens);
}
