#include "grammar.hpp"
#include <exception>

namespace Grammar
{

Grammar::Grammar::Grammar(std::vector<std::string> filenames)
{
    for (auto filename : filenames)
    {
        grammar_map.insert(std::make_pair(filename, SymbolicTokenParsers()));
    }

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
            std::string term = terms[0];
            if (term.find(".grm") != std::string::npos) 
            {
                std::cout << "Found grammar linkage" << std::endl;
                parsers.push_back(retrieve_grammar(term)); // Term will be a filename
            }
            else
            {
                parsers.push_back(typeParser(term));
            }
        }
        else
        {
            parsers.push_back(dualTypeParser(terms[0], terms[1]));
        }
    }
    return parsers;
}

SymbolicTokenParser Grammar::Grammar::retrieve_grammar(std::string filename)
{
    SymbolicTokenParser grammar_parser = [filename, this](SymbolicTokens tokens)
    {
        SymbolicTokenParser parser = typeParser("literal");

        auto search = grammar_map.find(filename);
        if (search != grammar_map.end())
        {
             parser = inOrder<SymbolicToken>(search->second);
        }
        else
        {
            std::cout << filename << " is not an element of the grammar map" << std::endl;
        }

        Result<SymbolicToken> result = parser(tokens);
        return result;
    };
    return grammar_parser;
}

}
