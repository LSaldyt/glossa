#include "../../IO/IO.hpp"
#include "../../Lex/Seperate.hpp"
#include "../../Syntax/Statements.hpp"
#include "../TokenParsers.hpp"

#include <string>
#include <tuple>

namespace Grammar
{

using namespace Parse;
using namespace Match;
using namespace Syntax;

//Build a parser from a grammar file
const auto read = [](std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    
    for (auto line : content)
    {
        std::cout << line << std::endl;
        auto terms = Lex::seperate(line, {std::make_tuple(" ", false)});
        if (terms.size() == 1)
        {
            parsers.push_back(typeParser(just(terms[0])));
        }
        else
        {
            parsers.push_back(dualTypeParser(just(terms[0]), just(terms[1]), true));
        }
    }
    return parsers;
};

const auto run = [](SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<SymbolicTokens> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result.parsed);
        }
        else
        {
            return std::make_tuple(false, results);
        }
    }

    return std::make_tuple(true, results);
};

}
