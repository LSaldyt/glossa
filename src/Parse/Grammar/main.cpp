#include "grammar.hpp"
#include "construct.hpp"
#include "run.hpp"

int main()
{
    using namespace Grammar;

    auto grammar = Grammar::Grammar({"assignment.grm", "expression.grm", "value.grm", "functioncall.grm"});

    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>(Operator("=")), "=", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(42)), "int", "literal"),
                             SymbolicToken(std::make_shared<Operator>(Operator("+")), "+", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(7)), "int", "literal"),
        };

    auto assignment_grammar = grammar.grammar_map["assignment.grm"];
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
                auto constructed = construction_map[result.annotation](fromTokens(result.consumed));
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

