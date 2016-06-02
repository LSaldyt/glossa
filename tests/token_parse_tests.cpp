#include "catch.hpp"
#include "../src/Utilities/Parse/Parse.hpp"


TEST_CASE( "Token Parsers work")
{
    using namespace Parse;
    using namespace Syntax;
    const auto intParser     = just("int");
    const auto literalParser = just("type");

    SECTION ("standard token parsers work")
    {
        std::vector<Token> input    = { Token( "42", "int", "type" ) };
        REQUIRE(subTypeParser<Token>(intParser)(input).result == true);
        REQUIRE(typeParser<Token>(literalParser)(input).result == true);
    }

    SECTION ("symbolic token parsers work")
    {
        std::vector<SymbolicToken> input    = { SymbolicToken( std::make_shared<Integer>(Integer(42)), "int", "type" ) };
        REQUIRE(subTypeParser<SymbolicToken>(intParser)(input).result == true);
        REQUIRE(typeParser<SymbolicToken>(literalParser)(input).result == true);
    }

    SECTION ("dual type parser works")
    {
        std::vector<SymbolicToken> input    = { SymbolicToken( std::make_shared<Integer>(Integer(42)), "int", "type" ) };
        REQUIRE(dualTypeParser<SymbolicToken>(literalParser, intParser)(input).result == true);
    }

    SECTION ("token parsers work in order - even if they are for different sections of the token")
    {
        std::vector<SymbolicToken> input    =
        {
            SymbolicToken( std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier" ),
            SymbolicToken( opGenerator("="),                              "=",          "operator" ),
            SymbolicToken( std::make_shared<Integer>(Integer(42)),        "int",        "type" )
        };

        const auto int_assignment_parser = inOrderTokenParser<SymbolicToken>({
            typeParser<SymbolicToken>(just("identifier")),
            subTypeParser<SymbolicToken>(just("=")),
            dualTypeParser<SymbolicToken>(literalParser, intParser)
        });

        REQUIRE(int_assignment_parser(input).result == true);
    }
}
