#pragma once
#include "Base/Locale.hpp"
#include "Base/Templates.hpp"
#include "../Syntax/Token.hpp"
#include "Base/TokenResult.hpp"

namespace Parse
{
    using namespace Syntax;
    //Convert a standard parseFunction to one that parses Tokens
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> subTypeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> typeParser(ParseFunction parser);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> dualTypeParser(ParseFunction typeParserFunc, ParseFunction subTypeParserFunc, bool byType);

    //inOrder for tokenTypes
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> inOrderTokenParser(std::vector<std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)>> parsers);
    std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> constructDualTypeParser(std::vector<std::tuple<ParseFunction, ParseFunction>> parser_pairs, bool byType);

    // Parse a list of functions in order, failing if any of them fail
    const auto inOrder = [](ParseFunctions parsers)
    {
        auto parse = [parsers](const Terms& original_terms)
        {
            Terms parsed;
            Terms terms = original_terms;

            for (auto parser : parsers)
            {
                auto parse_result = parser(terms);
                if(parse_result.result)
                {
                    parsed.insert( parsed.end(), parse_result.parsed.begin(), parse_result.parsed.end() );
                    terms  = parse_result.remaining;
                }
                else
                {
                    return Result (false, Terms(), original_terms);
                }
            }
            return Result (true, parsed, terms);
        };
        return parse;
    };

    //Parse a single string, exactly
    const auto just = [](std::string value)
    {
        auto comparator = [value](Term term){ return value == term; };
        return singleTemplate(comparator);
    };

    // Change the success of a parser
    const auto inverse = [](ParseFunction parser)
    {
        return parseTemplate([parser](Terms terms)
        {
            auto result = parser(terms);
            result.result = !result.result;
            return result;
        });
    };

    // Attempt to parse any parser from a list of Parsers, failing only if all of the parsers fail, and passing if any of them pass
    const auto anyOf = [](ParseFunctions functions)
    {
        return parseTemplate([functions](Terms terms)
        {
            auto result = Result(false, Terms(), terms);
            for (auto function : functions)
            {
                auto func_result = function(terms);
                if(func_result.result)
                {
                    result = func_result;
                    break;
                }
            }
            return result;
        });
    };

    //Parse all parsers from a list of parsers, passing only if all of them pass
    const auto allOf = [](ParseFunctions functions)
    {
        return parseTemplate([functions](Terms terms)
        {
            auto result = Result(false, Terms(), terms);
            for (auto function : functions)
            {
                auto func_result = function(terms);
                if(!func_result.result)
                {
                    result = Result(false, Terms(), terms);
                    break;
                }
                else
                {
                    result = func_result;
                }
            }
            return result;
        });
    };

    // Convert a list of strings to a list of just(string)s
    const auto justFrom = [](std::vector<std::string> strings)
    {
        auto functions = ParseFunctions();
        functions.reserve(strings.size());
        for (auto s : strings)
        {
            functions.push_back(just(s));
        }
        return functions;
    };

    // Parse any term
    const auto wildcard = singleTemplate([](Term t) { return true; } );

    // Parse a term that is a subset of another term
    const auto subsetOf = [](std::string symbols)
    {
        return singleTemplate([symbols](Term term)
        {
            for(auto c : term)
            {
                if(symbols.empty() || (symbols.find_first_not_of(c) == std::string::npos))
                {
                    return false;
                }
            }
            return true;
        });
    };

    // Takes a parser and parses it repeatedly, never fails
    const auto many = [](ParseFunction parser)
    {
        Consumer consumer = [parser](Terms terms)
        {
            auto result = parser(terms);
            return Consumed(result.result, result.parsed);
        };
        return multiTemplate(consumer);
    };

    const auto sepBy = [](ParseFunction parser)
    {
        return inOrder({
        wildcard,
        many(inOrder({parser, wildcard}))
        });
    };

    //All of these are pretty self explanatory, they check a Term to see if it is a particular group of characters
    const auto digits = singleTemplate(is_digits);
    const auto alphas = singleTemplate(is_alphas);
    const auto puncts = singleTemplate(is_puncts);
    const auto uppers = singleTemplate(is_uppers);
    const auto lowers = singleTemplate(is_lowers);
}
