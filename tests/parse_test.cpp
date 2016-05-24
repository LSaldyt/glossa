#include "catch.hpp"
#include "../src/Utilities/Parser/Parse.hpp"
#include "../src/Utilities/Parser/Parser.hpp"

TEST_CASE( "Basic mathematical expressions can be parsed")
{
    using namespace Parse;
    Seperators mathematical = Seperators();
    mathematical.push_back(std::make_tuple('+', true));
    mathematical.push_back(std::make_tuple('-', true));
    mathematical.push_back(std::make_tuple('*', true));
    mathematical.push_back(std::make_tuple('/', true));

    std::vector<ParseFunction> functions;
    functions.push_back(many(wildcard));

    Parser<ParseFunction> parser(functions, mathematical);

    auto test_expression = "2*2+7";
    auto expected = Terms();
    expected.push_back("2");
    expected.push_back("*");
    expected.push_back("2");
    expected.push_back("+");
    expected.push_back("7");

    REQUIRE( parser.parse(test_expression).parsed == expected);
}
