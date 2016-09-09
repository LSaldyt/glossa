#include "grammar.hpp"
#include <exception>

namespace Grammar
{

Grammar::Grammar::Grammar(std::vector<std::string> filenames)
{
    for (auto filename : filenames)
    {
        grammar_map[filename] = read(filename);
    }
}

SymbolicTokenParser Grammar::Grammar::readGrammarTerms(std::vector<std::string> terms)
{
    if (terms.size() == 1)
    {
        std::string term = terms[0];
        if (term.find(".grm") != std::string::npos) 
        {
            std::cout << "Found grammar linkage to " << term << std::endl;
            return retrieve_grammar(term); // Term will be a filename
        }
        else
        {
            return typeParser(term);
        }
    }
    else
    {
        if (terms[0] == "many")
        {
            return many<SymbolicToken>(readGrammarTerms(std::vector<std::string>(terms.begin() + 1, terms.end()))); 
        }
        else if (terms[0] == "inOrder")
        {
            SymbolicTokenParsers ordered;
            for (auto term : std::vector<std::string>(terms.begin() + 1, terms.end()))
            {
                ordered.push_back(typeParser(term));
            }
            return inOrder<SymbolicToken>(ordered);
        }
        else
        {
            return dualTypeParser(terms[0], terms[1]);
        }
    }
}

SymbolicTokenParsers Grammar::Grammar::read(std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    
    for (auto line : content)
    {
        auto terms = Lex::seperate(line, {std::make_tuple(" ", false)});
        parsers.push_back(readGrammarTerms(terms));
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
