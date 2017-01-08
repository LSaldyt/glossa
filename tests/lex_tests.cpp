#include "catch.hpp"
#include "../src/lex/lex.hpp"
#include "../src/lex/lexmap.hpp"

TEST_CASE("The lexer and language modules work")
{
    using namespace match;
    using namespace lex;

    vector<string> keywords  = {"if", "then", "else", "is", "and", "not"};
    vector<string> operators = {"+", "-", "/", "*"};

    vector<LexMapTermSet> term_set;
    term_set.push_back(make_tuple(keywords, "keyword"));
    term_set.push_back(make_tuple(operators, "operator"));

    vector<LexMapLexer>  lexer_set;
    lexer_set.push_back(LexMapLexer(digits, "int", "type", 1));
    lexer_set.push_back(LexMapLexer(alphas, "identifier", "identifier", 5));

    const vector<Seperator> whitespace =
    {
        // make_tuple("    ", true),
        // make_tuple("\t", true),
        make_tuple("\t", false),
        make_tuple(" ",  false),
        make_tuple("\n", false)
    };
    LexMap test_language(term_set, lexer_set, whitespace);

    SECTION("lexing based on keywords, operators and whitespace")
    {
        auto test_sentence = "if 2 + 2 is 4 then 4 - 2 is 2";
        auto tokens        = lexWith(test_sentence, test_language, {'"'}, "#");
        REQUIRE(tokens[0].type     == "keyword");
        REQUIRE(tokens[0].values   == vector<string>{"if"});
        REQUIRE(tokens[1].sub_type == "int");
        REQUIRE(tokens[1].type     == "type");
        REQUIRE(tokens[1].values   == vector<string>{"2"});
        REQUIRE(tokens[2].type     == "operator");
        REQUIRE(tokens[2].values   == vector<string>{"+"});
    }
}
