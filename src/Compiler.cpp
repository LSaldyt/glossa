#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parse.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include "Utilities/Syntax/Token.hpp"
#include "Utilities/Lexer/Lexer.hpp"
#include "Utilities/Lexer/Seperate.hpp"
#include "Utilities/Lexer/Language.hpp"
#include "Utilities/Generator/Generator.hpp"
#include <iostream>

int main()
{
    using namespace Parse;
    using namespace Lexer;
    using namespace Syntax;
    using namespace Gen;

    Terms keywords  = {"if", "then", "else"};
    Terms operators = {"+", "-", "*", "/", "="};

    Language test_language(digits, alphas, keywords, operators);
    Lexer::Lexer lexer(test_language);

    auto parseFunctions = inOrder({just("identifier"), just("operator"), just("number")});
    auto parser = tokenParser<Token>(parseFunctions);

    SymbolicTokenParser symbolic_token_parser = tokenParser<SymbolicToken>(parseFunctions);

    Generator assignment_generator(symbolic_token_parser, AssignmentGenerator);

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        auto tokens = lexer.lex(line);
        SymbolicTokens symbolic_tokens = toSymbolic(parser(tokens).parsed);
        auto generated = assignment_generator.generate(symbolic_tokens);
        std::cout << std::get<0>(generated) << std::endl;
        std::cout << std::get<1>(generated) << std::endl;
    }
}
