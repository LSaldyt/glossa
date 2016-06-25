#include "Compiler.hpp"



int main()
{
    using namespace Compiler;

    SymbolicTokens test_tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("foo")), "", "identifier")};
    auto result = buildThing(test_tokens);
    //std::cout << "Thing: " << result.identifier << std::endl;

    Terms keywords  = {"return", "test"};
    Terms operators = {"+", "-", "*", "/", "=", "(", ")", ":"};

    Lex::LanguageTermSets term_set;
    term_set.push_back(std::make_tuple(keywords, "keyword"));
    term_set.push_back(std::make_tuple(operators, "operator"));

    Lex::LanguageParsers  parser_set;
    parser_set.push_back(LanguageParser(Parse::digits, "int", "type"));
    parser_set.push_back(LanguageParser(Parse::alphas, "", "identifier"));

    Lex::Language test_language(term_set, parser_set);

    auto content         = readFile     ("../input/input.txt");
    auto tokens          = tokenPass    (content, test_language);
    auto symbolic_tokens = symbolicPass (tokens);
    auto joined_tokens   = join         (symbolic_tokens);

    for(auto jt : joined_tokens)
    {
        std::cout << jt.type << " " << jt.sub_type << std::endl;
    }
}

namespace Compiler
{
    std::vector<Tokens> tokenPass(std::vector<std::string> content, const Language& language)
    {
        std::vector<Tokens> tokens;
        for (auto line : content)
        {
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
            tokens.push_back(SymbolicToken(std::make_shared<NewLine>(NewLine()), "\n", "\n"));
        }
        return tokens;
    }
}
