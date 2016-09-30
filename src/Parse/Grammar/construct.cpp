#include "grammar.hpp"
#include "construct.hpp"
#include "run.hpp"

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

void {
    auto run_result = run(std::get<0>(assignment_grammar), tokens);

    if (std::get<0>(run_result))
    {
        std::cout << "Parsing successful, collecting results" << std::endl;
        
        std::vector<std::shared_ptr<Symbol>> result_symbols;

        auto construction_indices = std::get<1>(assignment_grammar);
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
                auto constructed = construction_map[result.annotation](result.consumed);
                std::cout << constructed->representation() << std::endl;
                result_symbols.push_back(constructed);
            }
        }


        Assignment a(result_symbols);
        std::cout << a.representation() << std::endl;

    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
}

}
