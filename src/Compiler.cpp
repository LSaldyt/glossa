#include "Compiler.hpp"

int main()
{
    using namespace Compiler;

    auto expression_parser = inOrder({
        just("type"),
        many(inOrder({just("operator"), just("type")}))
    });

    Gen::SymbolicStatementParser assign_parser = std::make_tuple(makeTypeParser({
        just("identifier"),
        just("operator"),
        expression_parser,
        just("\n")
    }), Syntax::AssignmentGenerator);


    Gen::SymbolicStatementParser function_parser = std::make_tuple(inOrderTokenParser<SymbolicToken>({
        typeParser     <SymbolicToken> (just("identifier")),
        dualTypeParser <SymbolicToken> (just("operator"), just("(")),
        dualTypeParser <SymbolicToken> (just("operator"), just(")")),
        dualTypeParser <SymbolicToken> (just("operator"), just(":")),
        typeParser     <SymbolicToken> (just("\n")),
        dualTypeParser <SymbolicToken> (just("keyword"), just("return")),
        typeParser     <SymbolicToken> (just("type")),
        typeParser     <SymbolicToken> (just("\n")),
        }), Syntax::FunctionGenerator);

    Terms keywords  = {"return", "test"};
    Terms operators = {"+", "-", "*", "/", "=", "(", ")", ":"};

    Gen::SymbolicStatementParsers statement_parsers;
    statement_parsers.push_back(assign_parser);
    statement_parsers.push_back(function_parser);

    Lex::LanguageTermSets term_set;
    term_set.push_back(std::make_tuple(keywords, "keyword"));
    term_set.push_back(std::make_tuple(operators, "operator"));

    Lex::LanguageParsers  parser_set;
    parser_set.push_back(LanguageParser(Parse::digits, "int", "type"));
    parser_set.push_back(LanguageParser(Parse::alphas, "", "identifier"));

    Lex::Language test_language(term_set, parser_set, statement_parsers);

    auto content         = readFile     ("../input/input.txt");
    auto tokens          = tokenPass    (content, test_language);
    auto symbolic_tokens = symbolicPass (tokens);
    auto joined_tokens   = join         (symbolic_tokens);

    auto output = generate(test_language.language_generator, joined_tokens);
    for(auto o : output)
    {
        std::cout << o << "\n";
    }
    writeFile(output, "../output/output.cpp");
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
