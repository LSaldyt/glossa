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

    auto grammar = Grammar::Grammar({"assignment.grm", "expression.grm"});

    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>(Operator("=")), "=", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(42)), "int", "literal"),
                             SymbolicToken(std::make_shared<Operator>(Operator("+")), "+", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(7)), "int", "literal")};

    auto results = run(grammar.grammar_map["assignment.grm"], tokens);
    if (std::get<0>(results))
    {
        std::cout << "Parsed assignment from grammar file" << std::endl;
        for (auto tokens : std::get<1>(results))
        {
            for (auto t : tokens)
            {
                std::cout << t.value->representation() << std::endl;
            }
        }
    }
}

