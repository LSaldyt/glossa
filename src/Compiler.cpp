#include "Compiler.hpp"


int main()
{
    using namespace Compiler;

    auto grammar = Grammar::Grammar({"assignment", "expression", "value", "functioncall"}, "grammars/python/");

    Terms operators = {"+", "-", "*", "/", "=", ":"};
    Terms logicaloperators = {"&", "|"};
    Terms punctuators = {",", "(", ")"};

    LanguageTermSets term_sets;
    term_sets.push_back(make_tuple(grammar.keywords,  "keyword"));
    term_sets.push_back(make_tuple(operators, "operator"));
    term_sets.push_back(make_tuple(logicaloperators, "logicaloperator"));
    term_sets.push_back(make_tuple(punctuators, "punctuator"));

    LanguageLexers lexer_set = {
        LanguageLexer(digits, "int", "literal", 3),
        LanguageLexer(startswith("\""), "string", "string", 1),
        LanguageLexer(alphas, "identifier", "identifier", 3)};

    Language test_language(term_sets, lexer_set);
    grammar.language = test_language;

    auto content         = readFile     ("input/input.txt");
    auto tokens          = tokenPass    (content, grammar.language); 
    auto symbolic_tokens = symbolicPass (tokens);
    auto joined_tokens   = join         (symbolic_tokens);

    for(auto jt : joined_tokens)
    {
        print("Joined Token: " + jt.type + ", " + jt.sub_type);
    }

    auto symbols = grammar.constructFrom(joined_tokens);
    for (auto s : symbols)
    {
        print(s->representation());
    }
}

namespace Compiler
{
    std::vector<Tokens> tokenPass(std::vector<std::string> content, const Language& language)
    {
        std::vector<Tokens> tokens;
        for (auto line : content)
        {
            print("Lexing: " + line);
            tokens.push_back(lex(line, language));
        }
        return tokens;
    }

    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens)
    {
        std::vector<SymbolicTokens> symbolic_tokens;
        for (auto token_group : tokens)
        {
            symbolic_tokens.push_back(toSymbolic(generatorMap, token_group));
        }
        return symbolic_tokens;
    }

    SymbolicTokens join(std::vector<SymbolicTokens> token_groups)
    {
        auto tokens = SymbolicTokens();
        for (auto token_group : token_groups)
        {
            for (auto t : token_group)
            {
                tokens.push_back(t);
            }
        }
        return tokens;
    }
}
