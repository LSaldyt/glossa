#pragma once
#include "Base/Locale.hpp"
#include "Base/Templates.hpp"

namespace Match 
{

    /*
    template <typename T>
    Result<T> inOrderP (std::vector<std::function<Result<T>(std::vector<T>)>> matchers)
    {
        return [matchers](const std::vector<T>& original_terms)
        {
            std::vector<T> consumed;
            std::vector<T> terms(original_terms);

            for (auto matcher : matchers)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    consumed.insert(consumed.end(), result.consumed.begin(), result.consumed.end());
                    terms = result.remaining;
                }
                else
                {
                    return Result<T>(false, std::vector<T>(), original_terms);
                }
            }
            return Result<T>(true, consumed, terms);
        };
    }
    */

    // Parse a list of functions in order, failing if any of them fail
    const auto inOrder = [](MatchFunctions matchers)
    {
        auto match = [matchers](const Terms& original_terms)
        {
            Terms consumed;
            Terms terms = original_terms;

            for (auto matcher : matchers)
            {
                auto match_result = matcher(terms);
                if(match_result.result)
                {
                    consumed.insert( consumed.end(), match_result.consumed.begin(), match_result.consumed.end() );
                    terms  = match_result.remaining;
                }
                else
                {
                    return Result (false, Terms(), original_terms);
                }
            }
            return Result (true, consumed, terms);
        };
        return match;
    };

    //Parse a single string, exactly
    const auto just = [](std::string value)
    {
        auto comparator = [value](Term term){ return value == term; };
        return singleTemplate(comparator);
    };

    //Parse a single string, exactly
    const auto startswith = [](std::string value)
    {
        auto comparator = [value](Term term){ return std::string(term.begin(), term.begin() + value.size()) == value; };
        return singleTemplate(comparator);
    };

    // Change the success of a matcher
    const auto inverse = [](MatchFunction matcher)
    {
        return matchTemplate([matcher](Terms terms)
        {
            auto result = matcher(terms);
            result.result = !result.result;
            return result;
        });
    };

    // Attempt to parse any matcher from a list of matchers, failing only if all of the matchers fail, and passing if any of them pass
    const auto anyOf = [](MatchFunctions functions)
    {
        return matchTemplate([functions](Terms terms)
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

    //Parse all matchers from a list of matchers, passing only if all of them pass
    const auto allOf = [](MatchFunctions functions)
    {
        return matchTemplate([functions](Terms terms)
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
        auto functions = MatchFunctions();
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

    // Takes a matcher and parses it repeatedly, never fails
    const auto many = [](MatchFunction matcher)
    {
        Consumer consumer = [matcher](Terms terms)
        {
            auto result = matcher(terms);
            return Consumed(result.result, result.consumed);
        };
        return multiTemplate(consumer);
    };

    const auto sepBy = [](MatchFunction sep, MatchFunction val=wildcard)
    {
        return inOrder({
        val,
        many(inOrder({sep, val}))
        });
    };

    //All of these are pretty self explanatory, they check a Term to see if it is a particular group of characters
    const auto digits = singleTemplate(is_digits);
    const auto alphas = singleTemplate(is_alphas);
    const auto puncts = singleTemplate(is_puncts);
    const auto uppers = singleTemplate(is_uppers);
    const auto lowers = singleTemplate(is_lowers);
}
