#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/Utilities/Parser/Parse.hpp"

TEST_CASE( "Tokenizer acts as expected")
{
    auto test_tokens = Parse::Tokens();
    test_tokens.push_back("hello");
    REQUIRE(test_tokens == Parse::tokenize("hello"));
}
