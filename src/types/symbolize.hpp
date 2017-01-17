#pragma once
#include "import.hpp"
#include "../syntax/symbols/export.hpp"
#include "../tools/tools.hpp"

/**
 * Constructs low-level (AST leaf) symbolic tokens from a vector of tokens
 * Requires relevant construction functions
 */
const auto toSymbolic = [](std::unordered_map<std::string, syntax::SymbolGenerator> generatorMap, Tokens tokens)
{
    std::vector<SymbolicToken> symbolic_tokens;
    symbolic_tokens.reserve(tokens.size());
    for (auto token : tokens)
    {
        auto type = token.type;
        auto search = generatorMap.find(type);
        if (search != generatorMap.end())
        {
            std::string text;
            for (auto v : token.values)
            {
                text += v;
            }
            auto symbolic = SymbolicToken(search->second(token.values), token.sub_type, type, text);
            symbolic_tokens.push_back(symbolic);
        }
        else
        {
            std::string representation;
            for (auto v : token.values)
            {
                representation += v + ",";
            }
            tools::print("Failed to generate type from \"" + representation + "\", (type: " + token.type + "), (subtype: " + token.sub_type + ")" );
        }
    }

    return symbolic_tokens;
};
