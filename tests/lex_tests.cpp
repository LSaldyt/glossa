#include "catch.hpp"
#include "../src/Lex/Lex.hpp"
#include "../src/Language.hpp"

TEST_CASE("The lexer and language modules work")
{
    using namespace Parse;
    using namespace Lex;

    Terms keywords  = {"if", "then", "else", "is", "and", "not"};
    Terms operators = {"+", "-", "/", "*"};

    Lex::LanguageTermSets term_set;
    term_set.push_back(std::make_tuple(keywords, "keyword"));
    term_set.push_back(std::make_tuple(operators, "operator"));

    Lex::LanguageParsers  parser_set;
    parser_set.push_back(LanguageParser(Parse::digits, "int", "type"));
    parser_set.push_back(LanguageParser(Parse::alphas, "", "identifier"));
    Gen::SymbolicStatementParsers statement_parsers;

    Lex::Language test_language(term_set, parser_set, statement_parsers);

    SECTION("Lexing based on keywords, operators and whitespace")
    {
        auto test_sentence = "if 2 + 2 is 4 then 4 - 2 is 2";
        auto tokens = lex(test_sentence, test_language);
        REQUIRE(tokens[0].type     == "keyword");
        REQUIRE(tokens[0].value    == "if");
        REQUIRE(tokens[1].sub_type == "int");
        REQUIRE(tokens[1].type     == "type");
        REQUIRE(tokens[1].value    == "2");
        REQUIRE(tokens[2].type     == "operator");
        REQUIRE(tokens[2].value    == "+");
    }
}
