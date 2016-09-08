#include "grammar.hpp"

namespace Grammar
{

Grammar::Grammar::Grammar(std::vector<std::string> filenames)
{
    for (auto filename : filenames)
    {
        grammar_map.insert(std::make_pair(filename, read(filename)));
    }
}

SymbolicTokenParsers Grammar::Grammar::read(std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    
    for (auto line : content)
    {
        auto terms = Lex::seperate(line, {std::make_tuple(" ", false)});
        if (terms.size() == 1)
        {
            parsers.push_back(typeParser(terms[0]));
        }
        else
        {
            parsers.push_back(dualTypeParser(terms[0], terms[1]));
        }
    }
    return parsers;
}

}
