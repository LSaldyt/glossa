#include "grammar.hpp"

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
        auto constructed = grammar.construct(name, std::get<1>(result)); 
        std::cout << "Constructed: " << std::endl;
        std::cout << constructed->representation() << std::endl;
    }
}

