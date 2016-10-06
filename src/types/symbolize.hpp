#pragma once
#include "import.hpp"
#include "../syntax/symbols.hpp"
#include "../tools/tools.hpp"

const auto toSymbolic = [](std::unordered_map<std::string, syntax::SymbolGenerator> generatorMap, Tokens tokens)
{
    SymbolicTokens symbolic_tokens;
    symbolic_tokens.reserve(tokens.size());
    for (auto token : tokens)
    {
        auto type = token.type;
        auto search = generatorMap.find(type);
        if (search != generatorMap.end())
        {
            auto symbolic = SymbolicToken(search->second(token.values), token.sub_type, type);
            symbolic_tokens.push_back(symbolic);
        }
        else
        {
            tools::print("Failed to generate type ", token.type);
        }
    }

    return symbolic_tokens;
};
