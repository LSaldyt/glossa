#include "catch.hpp"
#include "../src/Match/Match.hpp"

TEST_CASE( "Basic matching works")
{
    using namespace Match;
    Terms expected;

    auto hello = just("hello");

    SECTION ("standard matching works")
    {
        expected = {"hello"};
        REQUIRE(hello(expected).consumed        == expected);
        REQUIRE(wildcard(expected).consumed             == expected);
        REQUIRE(subsetOf("ehlo")(expected).consumed     == expected);
        MatchFunctions matchers = {hello, wildcard, subsetOf("ehlo")};
        REQUIRE(anyOf(matchers)(expected).consumed       == expected);
        REQUIRE(inverse(just("nothello"))({"hello"}).result == true);
        REQUIRE(alphas(expected).consumed               == expected);
        REQUIRE(lowers(expected).consumed               == expected);
        REQUIRE(startswith("h")(expected).consumed == expected);
    }

    SECTION ("more advanced matching works")
    {
        expected = {"hello", "hello"};
        REQUIRE(many(hello)(expected).consumed  == expected);
        REQUIRE(inOrder({hello, hello})(expected).consumed == expected);

        REQUIRE(allOf({hello, wildcard})({"hello"}).result         == true);
        REQUIRE(allOf({hello, just("nothello")})({"hello"}).result == false);

        REQUIRE(inOrder(justFrom({"hello", "hello"}))(expected).consumed     == expected);
    }

    SECTION ("sepBy")
    {
        expected = {"a", ",", "b"};
        REQUIRE(sepBy(just(","), alphas)(expected).consumed == expected); // Wont discard the comma, but will recognize it as valid
    }

    SECTION("digits")
    {
        expected = {"123"};
        REQUIRE(digits(expected).consumed               == expected);
    }

    SECTION("uppers")
    {
        expected = {"HELLO"};
        REQUIRE(uppers(expected).consumed               == expected);
    }

    SECTION("operators")
    {
        expected = {"+"};
        REQUIRE(subsetOf("+-/*")(expected).consumed              == expected);
    }

    SECTION("punctuation")
    {
        expected = {"!?"};
        REQUIRE(puncts(expected).consumed               == expected);
    }
}

/*
 *
namespace Match 
{

    const auto sepBy = [](MatchFunction sep, MatchFunction val=wildcard)
    {
        return inOrder({
        val,
        many(inOrder({sep, val}))
        });
    };

 * */
