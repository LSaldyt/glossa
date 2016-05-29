#pragma once
#include <tuple>
#include <string>
#include <iostream>
#include "Symbols.hpp"
#include "../Parse/Base/Types.hpp"

namespace Syntax
{
    using Token          = std::tuple<std::string, std::string>;
    using Tokens         = std::vector<Token>;
    using SymbolicToken  = std::tuple<std::shared_ptr<Symbol>, std::string>;
    using SymbolicTokens = std::vector<SymbolicToken>;

    const auto toSymbolic = [](Tokens tokens)
    {
        SymbolicTokens symbolic_tokens;
        for(auto token : tokens)
        {
            auto type = std::get<1>(token);
            auto search = generatorMap.find(type); //generatorMap is defined in Symbols
            if (search != generatorMap.end())
            {
                auto symbolic =
                    std::make_tuple(search->second(std::get<0>(token)), type);
                symbolic_tokens.push_back(symbolic);
            }
            else
            {
                std::cout << "Failed to generate invalid Type\n";
            }
        }
        return symbolic_tokens;
    };
}
