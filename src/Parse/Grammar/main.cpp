#include "construct.hpp"

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

}

