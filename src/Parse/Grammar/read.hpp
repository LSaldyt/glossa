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

const auto buildConstructor = [](std::vector<std::string> terms)
{
    return [terms](std::vector<SymbolicTokens> tokenlists){
        std::string representation;
        
        for (auto t : terms)
        {
            //std::cout << t << std::endl;
            // Locations meant to be replaced by a token's representation
            if (t[0] == '[')
            {
                std::cout << "Found Indices:" << std::endl;
                auto indices = Lex::seperate(t, {std::make_tuple("[", false), std::make_tuple("]", false)});
                for (auto index : indices)
                {
                    std::cout << std::stoi(index) << std::endl;
                }

                int i      = std::stoi(indices[0]);
                int j      = std::stoi(indices[1]);
                auto token = tokenlists[i][j];

                std::cout << "Representation of type: " << token.type << ", subtype: " << token.sub_type << std::endl;


                std::cout << token.value->representation() << std::endl;
                representation.append(token.value->representation());
                
            }
            else
            {
                representation.append(t);
            }
            representation.append(" ");
        }

        return representation;
    };
};

//Build a parser from a grammar file
const auto read = [](std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    auto keep = *(content.end() - 1);
    auto keepterms = Lex::seperate(keep, {std::make_tuple(" ", false)});
    std::cout << "Constructor: " << std::endl;
    auto constructor = buildConstructor(keepterms);
    
    if (content.size() > 0)
        content = Terms(content.begin(), content.end() - 1);
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
    return std::make_tuple(parsers, constructor);
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
