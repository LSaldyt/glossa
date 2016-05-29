#include "Compiler.hpp"


int main()
{
    using namespace Compiler;

    auto parseFunctions = inOrder({just("identifier"), just("operator"), just("int")});
    auto parser = tokenParser<Token>(parseFunctions);

    SymbolicTokenParser symbolic_token_parser = tokenParser<SymbolicToken>(parseFunctions);

    Generator assignment_generator(symbolic_token_parser, AssignmentGenerator);

    auto content         = readFile     ("input.txt");
    auto tokens          = tokenPass    (content);
    auto symbolic_tokens = symbolicPass (tokens);

    std::vector<std::string> output;
    for (auto symbolic_token_group : symbolic_tokens)
    {
        auto generated = assignment_generator.generate(symbolic_token_group);
        output.push_back(std::get<0>(generated));
    }
    writeFile(output, "output.cpp");
}

namespace Compiler
{
    std::vector<Tokens> tokenPass(std::vector<std::string> content)
    {
        std::vector<Tokens> tokens;
        for (auto line : content)
        {
            tokens.push_back(lex(line, test_language));
        }
        return tokens;
    }

    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens)
    {
        std::vector<SymbolicTokens> symbolic_tokens;
        for (auto token_group : tokens)
        {
            symbolic_tokens.push_back(toSymbolic(token_group));
        }
        return symbolic_tokens;
    }
}
