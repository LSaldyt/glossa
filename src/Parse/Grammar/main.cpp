#include "construct.hpp"

int main()
{
    using namespace Grammar;

    auto grammar = Grammar::Grammar({"assignment", "expression", "value", "functioncall"}, "python/");

    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>  (Operator("=")),   "=",          "operator"),
                             SymbolicToken(std::make_shared<Integer>   (Integer(42)),     "int",        "literal"),
                             SymbolicToken(std::make_shared<Operator>  (Operator("+")),   "+",          "operator"),
                             SymbolicToken(std::make_shared<Integer>   (Integer(7)),      "int",        "literal")
        };


    while (tokens.size() > 0)
    {
        auto result = grammar.identify(tokens);
        auto name   = std::get<0>(result);
        auto construction_indices = std::get<1>(grammar.grammar_map[name]);

        auto constructed = construct(std::get<1>(result), construction_indices); 
        std::cout << "Constructed: " << std::endl;
        std::cout << constructed->representation() << std::endl;
    }
}

