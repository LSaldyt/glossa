#include "../../IO/IO.hpp"
#include "../../Lex/Seperate.hpp"
#include "../TokenParsers.hpp"
//std::vector<std::string> readFile(std::string filename);

#include <string>

//Build a parser from a grammar file
const auto read = [](std::string filename)
{
    SymbolicTokenParsers parsers;
    for (auto line : readFile(filename))
    {
        std::cout << "Reading: " << line << std::endl;
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
    return inOrder(parsers);
};

