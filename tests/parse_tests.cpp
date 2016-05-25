#include "catch.hpp"
#include "../src/Utilities/Parser/Parse.hpp"

TEST_CASE( "Basic Parsers work")
{
    using namespace Parse;
    Terms expected;

    SECTION ("standard parsers work")
    {
        expected = {"hello"};
        REQUIRE(just("hello")(expected).parsed        == expected);
        REQUIRE(wildcard(expected).parsed             == expected);
        REQUIRE(subsetOf("ehlo")(expected).parsed     == expected);
        ParseFunctions parsers = {just("hello"), wildcard, subsetOf("ehlo")};
        REQUIRE(anyOf(parsers)(expected).parsed == expected);
        REQUIRE(alphas(expected).parsed               == expected);
        REQUIRE(lowers(expected).parsed               == expected);
    }

    SECTION("digits")
    {
        expected = {"123"};
        REQUIRE(digits(expected).parsed               == expected);
    }

    SECTION("uppers")
    {
        expected = {"HELLO"};
        REQUIRE(uppers(expected).parsed               == expected);
    }

    SECTION("operators")
    {
        expected = {"+"};
        REQUIRE(parseOp(expected).parsed              == expected);
    }

    SECTION("punctuation")
    {
        expected = {"!?"};
        REQUIRE(puncts(expected).parsed               == expected);
    }

    SECTION("many")
    {
        expected = {"hello", "hello"};
        REQUIRE(many(just("hello"))(expected).parsed  == expected);
    }
}
