#include "StatementHelpers.hpp"

namespace Parse
{
    StatementParser statementBuilder(std::function<std::shared_ptr<Statement>(SymbolicTokens&)> builder);

    std::tuple<bool, std::shared_ptr<Statement>>              buildStatement (SymbolicTokens& tokens);
    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens);

    std::tuple<bool, std::shared_ptr<Symbol>> buildExpression(SymbolicTokens& tokens);

    const auto buildAssignment = statementBuilder([=](SymbolicTokens& tokens){
        Assignment a;
        bindIdent(a.identifier, tokens);
        advance(subTypeParser("="),   tokens);
        bindExpression(a.value, tokens);
        return std::make_shared<Assignment>(a);
        });

    const auto buildFunction = statementBuilder([=](SymbolicTokens& tokens){
        Function f;
        bindIdent(f.identifier,             tokens);
        advance(subTypeParser("("),               tokens);
        bindArgnames(f.argnames,            tokens);
        advance(subTypeParser(")"),               tokens);
        advance(subTypeParser(":"),               tokens);
        bindStatements(f.body,              tokens);
        advance(subTypeParser("return"),          tokens);
        bindExpression(f.return_expression, tokens);
        return std::make_shared<Function>(f);
    });


    const std::vector<StatementParser> statementBuilders = 
    {
        buildAssignment, buildFunction // Larger parsers should come first
    };
}
