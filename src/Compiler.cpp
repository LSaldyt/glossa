#include "Compiler.hpp"
#include "Utilities/IO/IO.hpp"
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

    auto parseFunctions = inOrder({just("identifier"), just("operator"), just("int")});
    auto parser = tokenParser<Token>(parseFunctions);

    SymbolicTokenParser symbolic_token_parser = tokenParser<SymbolicToken>(parseFunctions);

    Generator assignment_generator(symbolic_token_parser, AssignmentGenerator);

    std::vector<std::string> output;
    auto file = readFile("input.txt");
    for (auto line : file)
    {
        std::cout << "Compiled: " << line << std::endl;
        auto tokens = lexer.lex(line);
        SymbolicTokens symbolic_tokens = toSymbolic(parser(tokens).parsed);
        auto generated = assignment_generator.generate(symbolic_tokens);
        std::cout << "To: " << std::get<0>(generated) << std::endl;
        output.push_back(std::get<0>(generated));
    }
    writeFile(output, "output.cpp");
}
