#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parse.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include "Utilities/Lexer/Lexer.hpp"
#include "Utilities/Lexer/Seperate.hpp"
#include "Utilities/Lexer/Language.hpp"
#include <iostream>

int main()
{
    using namespace Parse;
    using namespace Lexer;
    using namespace Syntax;

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        Terms keywords  = {"if", "then", "else"};
        Terms operators = {"+", "-", "*", "/"};

        Language test_language(digits, alphas, keywords, operators);
        Lexer::Lexer lexer(test_language);

        auto tokens = lexer.lex(line);

        auto parseFunctions = {just("number"), just("operator"), just("number")};
        auto parser = tokenParser(inOrder(parseFunctions));

        std::vector<std::shared_ptr<Symbol>> symbols;

        for(auto token : parser(tokens).parsed)
        {
            if (std::get<1>(token) == "number")
            {
                symbols.push_back(intGenerator(std::get<0>(token)));
            }
            else if (std::get<1>(token) == "operator")
            {
                symbols.push_back(opGenerator(std::get<0>(token)));
            }
        }
    }
}
