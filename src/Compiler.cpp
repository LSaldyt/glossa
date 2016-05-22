#include "Compiler.hpp"
#include "Utilities/IO/readFile.hpp"
#include "Utilities/Parser/Parser.hpp"
#include <iostream>

int main()
{
    using namespace Parse;

    Seperators whitespace = Seperators();
    whitespace.push_back(std::make_tuple(' ',  false));
    whitespace.push_back(std::make_tuple('\n', true));
    whitespace.push_back(std::make_tuple('\t', true));

    auto test_string = "this is a test";
    auto tokens = seperate(test_string, whitespace);
    for (auto t : tokens)
    {
        std::cout << "-" << t << "-" << std::endl;
    }

    // auto file = readFile("test_file.txt");
    // for (auto line : file)
    // {
    //     //pass
    // }
    //
    // std::vector<ParseFunction> test_functions;
    // auto hello = just("hello");
    // auto manyhello = many(hello);
    // test_functions.push_back(manyhello);
    //
    // Parser<ParseFunction> parser(test_functions, "test_parser");
    // auto result = parser.parse("hello hello world");
    // std::cout << result.result << std::endl;
    // for (auto p : result.parsed)
    // {
    //     std::cout << p << std::endl;
    // }
}
