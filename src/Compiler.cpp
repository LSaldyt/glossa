#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include <iostream>

int main()
{
    using namespace Parse;

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        //pass
    }

    std::vector<ParseFunction> test_functions;
    auto hello = just("hello");
    auto manyhello = many(hello);
    test_functions.push_back(manyhello);

    Parser<ParseFunction> parser(test_functions, "test_parser");
    auto result = parser.parse("hello hello world");
    std::cout << result.result << std::endl;
    for (auto p : result.parsed)
    {
        std::cout << p << std::endl;
    }
}
