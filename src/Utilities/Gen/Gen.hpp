#pragma once
#include "Import.hpp"
#include "GenResult.hpp"

namespace Gen
{
    std::vector<std::string> generate(const Generator& generator, const SymbolicTokens& tokens);

    Generator makeGenerator(const SymbolicStatementParser&);
    Generator firstOf(const std::vector<Generator>& generators);
}
