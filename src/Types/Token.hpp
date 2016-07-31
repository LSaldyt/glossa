#pragma once
#include <tuple>
#include <string>
#include <iostream>
#include "../Syntax/Syntax.hpp"
#include "../Match/Base/Types.hpp"

namespace Syntax
{
    struct Token
    {
        std::string value;
        std::string sub_type;
        std::string type;
        Token(std::string set_value, std::string set_sub_type, std::string set_type)
        {
            value    = set_value;
            sub_type = set_sub_type;
            type     = set_type;
        }
    };
    using Tokens         = std::vector<Token>;
    struct SymbolicToken
    {
        std::shared_ptr<Symbol> value;
        std::string sub_type;
        std::string type;
        SymbolicToken(std::shared_ptr<Symbol> set_value, std::string set_sub_type, std::string set_type)
        {
            value    = set_value;
            sub_type = set_sub_type;
            type     = set_type;
        }
    };
    using SymbolicTokens = std::vector<SymbolicToken>;

    const auto toSymbolic = [](std::unordered_map<std::string, SymbolGenerator> generatorMap, Tokens tokens)
    {
        SymbolicTokens symbolic_tokens;
        symbolic_tokens.reserve(tokens.size());
        for (auto token : tokens)
        {
            auto type = token.type;
            auto search = generatorMap.find(type);
            if (search != generatorMap.end())
            {
                auto symbolic = SymbolicToken(search->second(token.value), token.sub_type, type);
                symbolic_tokens.push_back(symbolic);
            }
            else
            {
                std::cout << "Failed to generate type " << token.type << std::endl;
            }
        }

        return symbolic_tokens;
    };
}
