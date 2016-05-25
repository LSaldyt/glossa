#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parse.hpp"
#include "Utilities/Parser/ParserBase/Seperate.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include "Utilities/Lexer/Lexer.hpp"
#include <iostream>

int main()
{
    using namespace Parse;
    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        for(auto t : seperate(line, mathematical))
        {
            std::cout << "-" <<  t << "-" <<  std::endl;
        }
    }
}
