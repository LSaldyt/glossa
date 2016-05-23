#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include "Utilities/Syntax/Symbols.hpp"
#include <iostream>

int main()
{
    using namespace Parse;

    auto file = readFile("test_file.txt");

    std::vector<ParseFunction> functions;
    functions.push_back(many(any));

    Parser<ParseFunction> parser(functions, "test_parser", mathematical);

    for (auto line : file)
    {
        for (auto token : seperate(line, mathematical))
        {
            std::cout << token;
        }
        std::cout << std::endl;
    }
}
