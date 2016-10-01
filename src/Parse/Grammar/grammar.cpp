#include "grammar.hpp"
#include <exception>

namespace Grammar
{

Grammar::Grammar::Grammar(std::vector<std::string> filenames, std::string directory)
{
    for (auto filename : filenames)
    {
        grammar_map[filename] = read(directory + filename);
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
        throw std::exception();
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
        auto first   = terms[0];
        bool keep    = true;
        // If first of pair starts with !, discard its parse result
        if (first[0] == '!')
        {
            first = std::string(first.begin() + 1, first.end());
            keep = false;
        }
        // Allow linking to other grammar files
        if (first == "link")
        {
            parser = retrieveGrammar(terms[1]);
        }
        // Parse by type only
        else if (terms[1] == "wildcard")
        {
            parser = typeParser(first);
        }
        // Parse by a specific subtype (ex "keyword return")
        else
        {
            parser = dualTypeParser(first, terms[1]);
        }

        // Take care of a "!" if it was found early - make the parser discard its result
        if (not keep)
        {
            parser = discard(parser);
        }
    }
    else if (terms.size() > 2)
    {
        const auto keyword = terms[0];
        terms = std::vector<std::string>(terms.begin() + 1, terms.end());

        // Repeatedly parse a parser!
        if (keyword == "many")
        {
            parser = many<SymbolicToken>(readGrammarTerms(terms)); 
        }
        // Run several parsers in order, failing if any of them fail
        else if (keyword == "inOrder")
        {
            parser = inOrder<SymbolicToken>(readGrammarPairs(terms));
        }
        // Choose from several parsers
        else if (keyword == "anyOf")
        {
            parser = anyOf<SymbolicToken>(readGrammarPairs(terms));
        }
        else
        {
            std::cout << "Expected keyword..." << std::endl;
            throw std::exception();
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

std::tuple<SymbolicTokenParsers, std::vector<int>> Grammar::Grammar::read(std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    auto construct_line = content.back();
    content = std::vector<std::string>(content.begin(), content.end() - 1);
    
    for (auto line : content)
    {
        auto terms = Lex::seperate(line, {std::make_tuple(" ", false)});
        parsers.push_back(readGrammarTerms(terms));
    }

    std::vector<int> construct_indices;
    auto construct_terms = Lex::seperate(construct_line, {std::make_tuple(" ", false)});
    for (auto t : construct_terms)
    {
        construct_indices.push_back(std::stoi(t));
    }

    return std::make_tuple(parsers, construct_indices);
}

SymbolicTokenParser Grammar::Grammar::retrieveGrammar(std::string filename)
{
    SymbolicTokenParser grammar_parser = [filename, this](SymbolicTokens tokens)
    {
        SymbolicTokenParser parser = typeParser("literal");

        auto search = grammar_map.find(filename);
        if (search != grammar_map.end())
        {
             parser = inOrder<SymbolicToken>(std::get<0>(search->second));
        }
        else
        {
            std::cout << filename << " is not an element of the grammar map" << std::endl;
            throw std::exception();
        }

        Result<SymbolicToken> result = parser(tokens);
        result.annotation = filename;
        return result;
    };
    return grammar_parser;
}

}
