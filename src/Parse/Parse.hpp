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

    const auto identifierParser = typeParser(just("identifier"));
    const auto getRepr          = [](SymbolicToken s){return s.value->representation();};
    const auto getReprs         = [](SymbolicTokens tokens)
        {
            std::vector<std::string> strings;
            for (auto t : tokens)
            {
                strings.push_back(getRepr(t));
            }
            return strings;
        };

    const auto typeOrIdent = anyOf({just("type"), just("identifier")});
    const auto expression_parser = inOrder({
           typeOrIdent,
           many(inOrder({just("operator"), typeOrIdent}))
    });

    const auto everyOther = [](std::vector<std::string> terms)
    {
        std::vector<std::string> every_other;
        for (int i = 0; i < terms.size(); i++)
        {
            if (i % 2 == 0)
            {
                every_other.push_back(terms[i]);
            }
        }
        return every_other;
    };

    const auto compose = [](auto f, auto g)
    {
        return [f, g](auto x){ return f(g(x)); };
    };

    const auto commaSepList = builder<std::vector<std::string>>(subTypeParser(sepBy(just(","))), compose(everyOther, getReprs));
    const auto genIdent = builder<std::string>(typeParser(just("identifier")), [](SymbolicTokens tokens){return getRepr(tokens[0]);});

    template <typename T>
    std::vector<std::tuple<T, T>> toPairs(std::vector<T> items)
    {
        std::vector<std::tuple<T, T>> pairs;

        auto it = items.begin();
        while(it != items.end())
        {
            if(it + 1 != items.end())
            {
                pairs.push_back(std::make_tuple(*it, *(it+1)));
                it += 2; //Iterate two items at a time
            }
            else
            {
                break;
            }
        }
        return pairs;
    }


    std::function<StatementResult(SymbolicTokens&)> statementBuilder(std::function<std::shared_ptr<Statement>(SymbolicTokens&)> builder);

    std::tuple<bool, std::shared_ptr<Statement>>              buildStatement (SymbolicTokens& tokens);
    std::tuple<bool, std::vector<std::shared_ptr<Statement>>> buildStatements(SymbolicTokens& tokens);

    std::tuple<bool, Expression> buildExpression(SymbolicTokens& tokens);
}
