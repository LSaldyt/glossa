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
        for(auto t : lexer.lex(line))
        {
            std::cout << std::get<0>(t) << std::endl;
            std::cout << std::get<1>(t) << std::endl;
            //std::cout << "-" <<  t << "-" <<  std::endl;
        }
    }
}
