#include "Lexer.hpp"

namespace Lexer
{
    Lexer::Lexer(const SymbolGenerators& set_symbols,
                 const ParseFunction     set_parser,
                 const Seperators&       set_seperators)
    {
        symbols = set_symbols;
        parser  = set_parser;
    }

    Lexer::~Lexer(){}

    LexResult Lexer::lex(const std::string& sentence)
    {
        auto terms = seperate(sentence, seperators);
        auto parse_result = parser(terms);
        return match(parse_result);
    }

    LexResult Lexer::match(Parse::Result parse_result)
    {
        LexResult result = { false, Syntax::Tokens()};

        if(parse_result.result)
        {
            auto tokens = Syntax::Tokens();
            for (unsigned i = 0; i < parse_result.parsed.size() && i < symbols.size(); i++)
            {
                tokens.push_back(std::make_tuple(parse_result.parsed[i], symbols[i]));
            }
            result = {true, tokens};
        }

        return result;
    }

}
