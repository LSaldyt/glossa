#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include <iostream>

int main()
{
    std::cout << "Starting" << std::endl;
    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        std::cout << line << std::endl;
    }

    Parser parser;
    auto result = parser.parse("hello cruel world");
}
