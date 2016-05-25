#include "catch.hpp"
#include "../src/Utilities/Parser/Parse.hpp"

TEST_CASE( "Basic mathematical expressions can be parsed")
{
    using namespace Parse;
    Seperators mathematical = Seperators();
    mathematical.push_back(std::make_tuple("+", true));
    mathematical.push_back(std::make_tuple("-", true));
    mathematical.push_back(std::make_tuple("*", true));
    mathematical.push_back(std::make_tuple("/", true));

    auto test_expression = "2*2+7";
    auto expected = Terms();
    expected.push_back("2");
    expected.push_back("*");
    expected.push_back("2");
    expected.push_back("+");
    expected.push_back("7");

    REQUIRE( many(wildcard)(seperate(test_expression, mathematical)).parsed == expected);
}
