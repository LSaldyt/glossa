#include "construct.hpp"

namespace Grammar
{

std::vector<std::shared_ptr<Symbol>> fromTokens(std::vector<SymbolicToken> tokens)
{
    std::vector<std::shared_ptr<Symbol>> symbols;
    symbols.reserve(tokens.size());

    for (auto t : tokens)
    {
        symbols.push_back(t.value);
    }

    return symbols;
}

std::shared_ptr<Symbol> construct(std::vector<Result<SymbolicToken>> results, std::vector<int> construction_indices)
{
    std::vector<std::shared_ptr<Symbol>> result_symbols;

    for (auto i : construction_indices)
    {
        auto result = results[i];
        if (result.annotation == "none")
        {
            if (result.consumed.size() == 1)
            {
                std::cout << result.consumed.back().value->representation() << std::endl; 
                result_symbols.push_back(result.consumed.back().value);
            }
            else
            {
                throw std::exception();
            }
        }
        else
        {
            auto constructed = find_constructor(result.annotation)(fromTokens(result.consumed));
            std::cout << constructed->representation() << std::endl;
            result_symbols.push_back(constructed);
        }
    }

    Assignment a(result_symbols);
    return std::make_shared<Assignment>(a);
}

}
