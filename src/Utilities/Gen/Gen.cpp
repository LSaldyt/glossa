#include "Gen.hpp"

namespace Gen
{
    std::vector<std::string> generate(const Generator& generator, const SymbolicTokens& tokens)
    {
        auto tokens_copy = tokens;
        std::vector<std::string> output;
        while (tokens.size() > 0)
        {
            GenResult result = generator(tokens_copy);
            if (!result.result)
            {
                break;
            }
            output.push_back(result.generated);
            tokens_copy = result.remaining;
        }
        return output;
    }

    Generator makeGenerator(const SymbolicStatementParser& parser)
    {
        return [parser](SymbolicTokens tokens)
        {
            auto result = std::get<0>(parser)(tokens);
            std::string output = "";
            if (result.result)
            {
                output = std::get<1>(parser)(result.parsed)->generator();
            }
            return GenResult(result.result, output, result.remaining);
        };
    }

    Generator firstOf(const std::vector<Generator>& generators)
    {
        return [generators](SymbolicTokens tokens)
        {
            auto result = GenResult(false, "", tokens);
            for (auto generator : generators)
            {
                auto gen_result = generator(tokens);
                if (gen_result.result)
                {
                    result = gen_result;
                    break;
                }
            }
            return result;
        };
    }
}
