#include "catch.hpp"
#include "../src/Match/Match.hpp"

TEST_CASE( "Basic matching works")
{
    using namespace Match;
    Terms expected;

    auto hello = just<std::string>("hello");

    const auto assertEqual = [](auto a, auto b)
    {
        REQUIRE((a == b));
    };

    SECTION ("standard matching works")
    {
        std::vector<std::string> expected = {"hello"};
        assertEqual(hello(expected).consumed , expected);
        assertEqual(wildcard<std::string>()(expected).consumed, expected);
        assertEqual(anyOf<std::string>({hello, wildcard<std::string>()})(expected).consumed, expected);
        assertEqual(inverse<std::string>(just<std::string>("nothello"))(expected).result, true);
        assertEqual(alphas(expected).consumed, expected);
        assertEqual(lowers(expected).consumed, expected);
        assertEqual(startswith("h")(expected).consumed, expected);
    }

    SECTION ("more advanced matching works")
    {
        expected = {"hello", "hello"};
        assertEqual(many(hello)(expected).consumed  , expected);
        assertEqual(inOrder<std::string>({hello, hello})(expected).consumed , expected);

        assertEqual(allOf<std::string>({hello, wildcard<std::string>()})({"hello"}).result         , true);
        assertEqual(allOf<std::string>({hello, just<std::string>("nothello")})({"hello"}).result , false);
    }

    SECTION ("sepBy")
    {
        expected = {"a", ",", "b"};
        assertEqual(sepBy<std::string>(just<std::string>(","), alphas)(expected).consumed , expected); // Wont discard the comma, but will recognize it as valid
    }

    SECTION("digits")
    {
        expected = {"123"};
        assertEqual(digits(expected).consumed               , expected);
    }

    SECTION("uppers")
    {
        expected = {"HELLO"};
        assertEqual(uppers(expected).consumed               , expected);
    }

    SECTION("punctuation")
    {
        expected = {"!?"};
        assertEqual(puncts(expected).consumed               , expected);
    }
}

