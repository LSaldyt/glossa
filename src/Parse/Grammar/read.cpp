#include "read.hpp"
#include "grammar.hpp"

namespace Grammar
{

std::tuple<bool, std::vector<SymbolicTokens>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<SymbolicTokens> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result.consumed);
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
    auto results = run(std::get<0>(assignment_grammar), tokens);
    if (std::get<0>(results))
    {
        std::cout << "Parsed assignment from grammar file" << std::endl;
        
        auto parsed = std::get<1>(results);
        std::vector<SymbolicTokens> construction_tokens;
        for (auto i : std::get<1>(assignment_grammar))
        {
            construction_tokens.push_back(parsed[i]); // Keep only the lines designated by the grammar file
        }

        for (auto tokens : parsed)
        {
            for (auto t : tokens)
            {
                std::cout << t.value->representation() << std::endl;
            }
        }

        Assignment assignment(construction_tokens);
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
}

