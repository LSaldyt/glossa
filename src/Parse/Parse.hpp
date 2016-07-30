#pragma once
#include "../Match/Match.hpp"
#include "TokenResult.hpp"
#include "../Syntax/Syntax.hpp"
#include "StatementResult.hpp"
#include "Typename.hpp"
#include <exception>

namespace Parse
{
    using namespace Syntax;
    using namespace Match;
    //Convert a standard parseFunction to one that parses Tokens
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> typeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);

    //inOrder for tokenTypes
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> inOrderTokenParser(std::vector<std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)>> parsers);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> constructDualTypeParser(std::vector<std::tuple<ParseFunction, ParseFunction>> parser_pairs, bool byType);

    using SymbolicTokenParser = std::function<TokenResult<SymbolicToken>(SymbolicTokens)>;
    using StatementParser     = std::function<StatementResult(SymbolicTokens)>;

    bool advance(SymbolicTokenParser function, SymbolicTokens& tokens);

    struct bad_bind : public std::exception 
    {
        std::string s;
        bad_bind(std::string s)
        {
            this->s = s;
        }
    };

    template < typename T >
    std::function<std::tuple<bool, T>(SymbolicTokens&)> builder (std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> function, std::function<T(SymbolicTokens)> converter)
    {
        std::function<std::tuple<bool, T>(SymbolicTokens&)> f = [function, converter](SymbolicTokens& tokens)
        {
            auto result = function(tokens);
            auto to_return = std::make_tuple(false, T());
            if (result.result)
            {
                tokens = SymbolicTokens(tokens.begin() + result.parsed.size(), tokens.end());
                to_return = std::make_tuple(true, converter(result.parsed));
            }
            return to_return;
        };
        return f;
    }

    template <typename T>
    void bindTo(T &t, std::function<std::tuple<bool, T>(SymbolicTokens&)> typeGenerator, SymbolicTokens& tokens)
    {
        auto result = typeGenerator(tokens);
        if (! std::get<0>(result))
        {
            throw bad_bind("Failed to bind to " + std::string(get_name<decltype(t)>().data)); 
        }
        else
        {
            t = std::get<1>(result);
        }
    }
}
