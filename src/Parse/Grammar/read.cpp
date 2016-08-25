#include "read.hpp"

int main()
{
    using namespace Grammar;

    auto assignment = read("assignment.grm");
    //auto result     = assignment({SymbolicToken(std::make_shared<Symbol>(Statement), "identifier", "identifier"),
    //                              SymbolicToken(std::make_shared<Symbol>(Statement), "=", "operator"),
    //                              SymbolicToken(std::make_shared<Symbol>(Statement), "int", "type")})
    //std::cout << std::get<0>(result); 
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
