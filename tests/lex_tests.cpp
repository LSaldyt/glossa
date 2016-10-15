#include "catch.hpp"
#include "../src/Lex/Lex.hpp"
#include "../src/Lex/Language.hpp"

TEST_CASE("The lexer and language modules work")
{
    using namespace Match;
    using namespace Lex;

    Terms keywords  = {"if", "then", "else", "is", "and", "not"};
    Terms operators = {"+", "-", "/", "*"};

    Lex::LanguageTermSets term_set;
    term_set.push_back(std::make_tuple(keywords, "keyword"));
    term_set.push_back(std::make_tuple(operators, "operator"));

    Lex::LanguageLexers  lexer_set;
    lexer_set.push_back(LanguageLexer(digits, "int", "type", 1));
    lexer_set.push_back(LanguageLexer(alphas, "identifier", "identifier", 5));

    Lex::Language test_language(term_set, lexer_set);

    SECTION("Lexing based on keywords, operators and whitespace")
    {
        auto test_sentence = "if 2 + 2 is 4 then 4 - 2 is 2";
        auto tokens = lex(test_sentence, test_language);
        REQUIRE(tokens[0].type     == "keyword");
        REQUIRE(tokens[0].values   == Terms{"if"});
        REQUIRE(tokens[1].sub_type == "int");
        REQUIRE(tokens[1].type     == "type");
        REQUIRE(tokens[1].values   == Terms{"2"});
        REQUIRE(tokens[2].type     == "operator");
        REQUIRE(tokens[2].values   == Terms{"+"});
    }
}
