#include "Compiler.hpp"

int main()
{
    using namespace Compiler;

    auto content         = readFile     ("input.txt");
    auto tokens          = tokenPass    (content);
    auto symbolic_tokens = symbolicPass (tokens);
    auto joined_tokens   = join(symbolic_tokens);

    std::vector<std::string> output;
    auto result = generate({}, joined_tokens);
    output.push_back(result.generated);
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

    SymbolicTokens join(std::vector<SymbolicTokens> token_groups)
    {
        auto tokens = SymbolicTokens();
        for (auto token_group : token_groups)
        {
            for (auto t : token_group)
            {
                tokens.push_back(t);
            }
            tokens.push_back(std::make_tuple(std::make_shared<NewLine>(NewLine()), "\n"));
        }
        return tokens;
    }
}
