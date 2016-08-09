#include "../Syntax/Symbols.hpp"

const auto toSymbolic = [](std::unordered_map<std::string, Syntax::SymbolGenerator> generatorMap, Tokens tokens)
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
