#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include <iostream>

int main()
{
    using namespace Parse;

    auto file = readFile("test_file.txt");

    Seperators mathematical = Seperators();
    mathematical.push_back(std::make_tuple('+', true));
    mathematical.push_back(std::make_tuple('-', true));
    mathematical.push_back(std::make_tuple('*', true));
    mathematical.push_back(std::make_tuple('/', true));

    std::vector<ParseFunction> functions;
    functions.push_back(many(any));

    Parser<ParseFunction> parser(functions, "test_parser", mathematical);


    for (auto line : file)
    {
        std::cout << line << std::endl;
        for (auto token : seperate(line, mathematical))
        {
            std::cout << token;
        }
        std::cout << std::endl;

        auto result = parser.parse(line);
        for (auto p : result.parsed)
        {
            std::cout << p << std::endl;
        }
    }
}
