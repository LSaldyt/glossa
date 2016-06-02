#include "catch.hpp"
#include "../src/Utilities/Parse/Parse.hpp"

TEST_CASE( "Basic Parsers work")
{
    using namespace Parse;
    Terms expected;

    auto hello = just("hello");

    SECTION ("standard parsers work")
    {
        expected = {"hello"};
        REQUIRE(hello(expected).parsed        == expected);
        REQUIRE(wildcard(expected).parsed             == expected);
        REQUIRE(subsetOf("ehlo")(expected).parsed     == expected);
        ParseFunctions parsers = {hello, wildcard, subsetOf("ehlo")};
        REQUIRE(anyOf(parsers)(expected).parsed       == expected);
        REQUIRE(inverse(just("nothello"))({"hello"}).result == true);
        REQUIRE(alphas(expected).parsed               == expected);
        REQUIRE(lowers(expected).parsed               == expected);
    }

    SECTION ("more advanced parsers work")
    {
        expected = {"hello", "hello"};
        REQUIRE(many(hello)(expected).parsed  == expected);
        REQUIRE(inOrder({hello, hello})(expected).parsed == expected);

        REQUIRE(allOf({hello, wildcard})({"hello"}).result         == true);
        REQUIRE(allOf({hello, just("nothello")})({"hello"}).result == false);

        REQUIRE(inOrder(justFrom({"hello", "hello"}))(expected).parsed     == expected);

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
        REQUIRE(subsetOf("+-/*")(expected).parsed              == expected);
    }

    SECTION("punctuation")
    {
        expected = {"!?"};
        REQUIRE(puncts(expected).parsed               == expected);
    }
}
