/// Copyright 2017 Lucas Saldyt
#pragma once
#include "import.hpp"
#include "../syntax/symbols/export.hpp"
#include "../tools/tools.hpp"

using tools::OutputManager;

/**
 * Constructs low-level (AST leaf) symbolic tokens from a vector of tokens
 * Requires relevant construction functions
 */
const auto toSymbolic = [](std::unordered_map<std::string, syntax::SymbolGenerator> generatorMap, Tokens tokens, OutputManager logger)
{
    std::vector<SymbolicToken> symbolic_tokens;
    symbolic_tokens.reserve(tokens.size());
    for (auto token : tokens)
    {
        logger.log("Symbolizing token (type: " + token.type + "), (sub_type: " + token.sub_type + ")", 2);
        std::string text;
        for (auto v : token.values)
        {
            text += v;
        }
        logger.log("Token text: \"" + text + "\"");
        auto search = generatorMap.find(token.type);
        if (search != generatorMap.end())
        {
            logger.log("Rules for symbolic creation found, creating symbolic token", 2);
            auto symbolic = SymbolicToken(search->second(token.values), token.sub_type, token.type, text, token.line);
            logger.log("Symbolic token creation finished", 2);
            symbolic_tokens.push_back(symbolic);
        }
        else
        {
            throw tools::named_exception("Failed to generate type from \"" + 
                                          text + "\", (type: " + token.type + "), " + 
                                          "(subtype: " + token.sub_type + ")");
        }
    }

    return symbolic_tokens;
};
