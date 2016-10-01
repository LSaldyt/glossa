#include "run.hpp"

namespace Grammar
{

std::tuple<std::string, std::vector<Result<SymbolicToken>>> 
run
(GrammarMap grammar_map, SymbolicTokens& tokens)
{
    SymbolicTokens tokens_copy(tokens);
    for (auto kv : grammar_map)
    {
        auto result = evaluateGrammar(std::get<0>(kv.second), tokens_copy);
        if (std::get<0>(result))
        {
            return std::make_tuple(kv.first, std::get<1>(result));
        }
        else
        {
            tokens_copy = tokens;
        }
    }

    throw std::exception();
}

std::tuple<bool, std::vector<Result<SymbolicToken>>> 
evaluateGrammar
(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
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
