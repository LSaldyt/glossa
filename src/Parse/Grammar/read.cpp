#include "read.hpp"

int main()
{
    using namespace Grammar;

    auto assignment = read("assignment.grm");
    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Symbol>(Statement()), "identifier", "identifier"),
                            SymbolicToken(std::make_shared<Symbol>(Statement()), "=", "operator"),
                            SymbolicToken(std::make_shared<Symbol>(Statement()), "int", "type")};

    auto results = run(assignment, tokens);
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
