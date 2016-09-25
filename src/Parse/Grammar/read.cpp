#include "read.hpp"
#include "grammar.hpp"

namespace Grammar
{

std::tuple<bool, std::vector<Result<SymbolicToken>>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<Result<SymbolicToken>> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result);
        }
        else
        {
            return std::make_tuple(false, results);
        }
    }

    return std::make_tuple(true, results);
};

}

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
        
        auto results = std::get<1>(run_result);
        for (auto result : results)
        {
            std::cout << result.annotation << std::endl;
            for (auto t : result.consumed)
            {
                std::cout << t.value->representation() << std::endl;
            }

        }
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
}

