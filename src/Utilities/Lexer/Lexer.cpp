#include "Lexer.hpp"


Lexer::Lexer(Syntax::Symbols set_symbols, Parse::Parser<Parse::ParseFunction> set_parser)
{
    symbols = set_symbols;
    parser  = set_parser;
}

Lexer::~Lexer(){}

LexResult Lexer::lex(const std::string& sentence)
{
    auto parse_result = parser(sentence);
    return match(parse_result);

}

LexResult Lexer::lex(Parse::Terms terms)
{
    auto parse_result = parser(terms);
    return match(parse_result);
}

LexResult Lexer::match(Parse::NamedResult parse_result)
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
