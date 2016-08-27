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

    std::cout << "Finished parsing grammar file:" << std::endl;

    auto results = run(std::get<0>(assignment), tokens);
    if (std::get<0>(results))
    {
        auto repr = std::get<1>(assignment)(std::get<1>(results));
        std::cout << repr << std::endl;
    }
}

/*
const auto buildAssignment = statementBuilder([=](SymbolicTokens& tokens){
    Assignment a;
    bindIdent(a.identifier, tokens);
    advance(subType("="),   tokens);
    bindExpression(a.value, tokens);
    return std::make_shared<Assignment>(a);
    });
*/
