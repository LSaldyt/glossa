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

std::shared_ptr<Symbol> construct(std::tuple<bool, std::vector<Result<SymbolicToken>>> run_result, std::vector<int> construction_indices)
{
    //auto run_result = run(std::get<0>(assignment_grammar), tokens);

    if (std::get<0>(run_result))
    {
        std::cout << "Parsing successful, collecting results" << std::endl;
        
        std::vector<std::shared_ptr<Symbol>> result_symbols;

        // auto construction_indices = std::get<1>(assignment_grammar);
        auto results = std::get<1>(run_result);
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
        std::cout << a.representation() << std::endl;
        return std::make_shared<Assignment>(a);
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
        throw std::exception();
    }
}

}
