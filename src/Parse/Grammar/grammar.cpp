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

SymbolicTokenParsers Grammar::Grammar::readGrammarPairs(std::vector<std::string>& terms)
{
    SymbolicTokenParsers parsers;

    if (terms.size() % 2 != 0)
    {
        std::cout << "Could not read type pairs:" << std::endl;
        for (auto t : terms)
        {
            std::cout << t << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < (terms.size() / 2); i++)
    {
        int x = i * 2;
        std::vector<std::string> pair(terms.begin() + x, terms.begin() + x + 2);
        parsers.push_back(readGrammarTerms(pair));
    }

    return parsers;
}

SymbolicTokenParser Grammar::Grammar::readGrammarTerms(std::vector<std::string>& terms)
{
    SymbolicTokenParser parser;

    if (terms.size() == 2)
    {
        if (terms[0] == "link")
        {
            parser = retrieveGrammar(terms[1]);
        }
        else if (terms[1] == "wildcard")
        {
            parser = typeParser(terms[0]);
        }
        else
        {
            parser = dualTypeParser(terms[0], terms[1]);
        }
    }
    else if (terms.size() > 2)
    {
        const auto keyword = terms[0];
        terms = std::vector<std::string>(terms.begin() + 1, terms.end());

        if (keyword == "many")
        {
            parser = many<SymbolicToken>(readGrammarTerms(terms)); 
        }
        else if (keyword == "inOrder")
        {
            parser = inOrder<SymbolicToken>(readGrammarPairs(terms));
        }
        else if (keyword == "anyOf")
        {
            parser = anyOf<SymbolicToken>(readGrammarPairs(terms));
        }
        else
        {
            std::cout << "Expected keyword..." << std::endl;
        }
    }
    else
    {
        std::cout << "Grammar file incorrectly formatted: " << std::endl;
        for (auto t : terms)
        {
            std::cout << t << " ";
        }
        std::cout << std::endl;
        throw std::exception();
    }

    return parser;
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

SymbolicTokenParser Grammar::Grammar::retrieveGrammar(std::string filename)
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
            throw std::exception();
        }

        Result<SymbolicToken> result = parser(tokens);
        return result;
    };
    return grammar_parser;
}

}
