#include "read.hpp"

int main()
{
    using namespace Grammar;

    auto assignment = read("assignment.grm");
    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>(Operator("=")), "=", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(42)), "int", "type")};
    for (auto t : tokens)
    {
        std::cout << t.value->representation() << std::endl;
    }

    auto results = run(assignment, tokens);
    if (std::get<0>(results))
    {
        std::cout << "Parsed assignment from grammar file" << std::endl;
    }
}

