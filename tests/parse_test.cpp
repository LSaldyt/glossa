#include "catch.hpp"
#include "../src/Utilities/Parser/Parse.hpp"

TEST_CASE( "Tokenizer acts as expected")
{
    auto test_tokens = Parse::Tokens();
    test_tokens.push_back("hello");
    REQUIRE(test_tokens == Parse::tokenize("hello"));
}
