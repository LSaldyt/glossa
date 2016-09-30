#include "run.hpp"

namespace Grammar
{

std::tuple<bool, std::vector<Result<SymbolicToken>>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<Result<SymbolicToken>> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result);
        }
        else
        {
            return std::make_tuple(false, results);
        }
    }

    return std::make_tuple(true, results);
};

}
