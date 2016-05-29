#pragma once
#include "Import.hpp"

namespace Gen
{
    using SymbolicTokenParser = std::function<TokenResult<SymbolicToken>(SymbolicTokens)>;

    class Generator
    {
        SymbolicTokenParser parser;
        StatementGenerator  statement_generator;

    public:
        Generator();
        Generator(const SymbolicTokenParser&, const StatementGenerator&);

        std::tuple<std::string, bool> generate(SymbolicTokens);
    };
}
