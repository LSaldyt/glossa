#include "../../IO/IO.hpp"
#include "../../Lex/Seperate.hpp"
#include "../../Syntax/Statements.hpp"
#include "../TokenParsers.hpp"
//std::vector<std::string> readFile(std::string filename);

#include <string>

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

}
