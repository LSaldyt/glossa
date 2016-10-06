#pragma once
#include "base/locale.hpp"
#include "base/templates.hpp"
#include "../types/result.hpp"

namespace match 
{
    template <typename T>
    function<Result<T>(vector<T>)>
    annotate
    (function<Result<T>(vector<T>)> parser, string annotation)
    {
        return [parser, annotation](vector<T> tokens)
        {
            auto result = parser(tokens);
            result.annotation = annotation;
            return result;
        };
    };


    template <typename T>
    function<Result<T>(vector<T>)> 
    inOrder 
    (vector<function<Result<T>(vector<T>)>> matchers)
    {
        return [matchers](const vector<T>& original_terms)
        {
            vector<T> consumed;
            vector<T> terms(original_terms);

            for (auto matcher : matchers)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    concat(consumed, result.consumed);
                    terms = result.remaining;
                }
                else
                {
                    return Result<T>(false, vector<T>(), original_terms);
                }
            }
            return Result<T>(true, consumed, terms);
        };
    }

    
    template <typename T>
    function<Result<T>(vector<T>)>
    just
    (T value)
    {
        auto comparator = [value](T term){ return value == term; };
        return singleTemplate<T>(comparator);
    };

    const auto startswith = [](string value)
    {
        auto comparator = [value](Term term){ 
            if (term.size() >= value.size())
            {
                return string(term.begin(), term.begin() + value.size()) == value; 
            }
            else
            {
                return false;
            }
        };
        return singleTemplate<Term>(comparator);
    };

    template <typename T>
    function<Result<T>(vector<T>)>
    optional
    (function<Result<T>(vector<T>)> matcher)
    {
        return [matcher](vector<T> terms)
        {
            auto result = matcher(terms);
            if (!result.result)
            {
                result.result = true;
            }
            return result;
        };
    }

    template <typename T>
    function<Result<T>(vector<T>)>
    inverse
    (function<Result<T>(vector<T>)> matcher)
    {
        return [matcher](vector<T> terms)
        {
            auto result = matcher(terms);
            result.result = !result.result;
            return result;
        };
    }


    // Attempt to parse any matcher from a list of matchers, failing only if all of the matchers fail, and passing if any of them pass
    template <typename T>
    function<Result<T>(vector<T>)>
    anyOf 
    (vector<function<Result<T>(vector<T>)>> matchers)
    {
        return [matchers](vector<T> terms)
        {
            auto result = Result<T>(false, vector<T>(), terms);
            for (auto matcher : matchers)
            {
                auto match_result = matcher(terms);
                if(match_result.result)
                {
                    result = match_result;
                    break;
                }
            }
            return result;
        };
    };


    //Parse all matchers from a list of matchers, passing only if all of them pass
    template <typename T>
    function<Result<T>(vector<T>)>
    allOf
    (vector<function<Result<T>(vector<T>)>> matchers)
    {
        return [matchers](vector<T> terms)
        {
            auto result = Result<T>(false, vector<T>(), terms);
            for (auto matcher : matchers)
            {
                auto match_result = matcher(terms);
                if(!match_result.result)
                {
                    result = Result<T>(false, vector<T>(), terms);
                    break;
                }
                else
                {
                    result = match_result;
                }
            }
            return result;
        };
    };

    // Parse any term
    template <typename T>
    function<Result<T>(vector<T>)>
    wildcard
    ()
    {
        return singleTemplate<T>([](T t){return true;});
    }

    // Takes a matcher and parses it repeatedly, never fails
    template <typename T>
    function<Result<T>(vector<T>)>
    many
    (function<Result<T>(vector<T>)> matcher)
    {
        return [matcher](vector<T> terms)
        {
            auto consumed = vector<T>(); 
            string annotation = "none";

            while(terms.size() > 0)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    annotation = result.annotation;
                    concat(consumed, result.consumed);
                    terms = slice(terms, result.consumed.size());
                }
                else
                {
                    break;
                }
            }

            auto result = Result<T>(true, consumed, terms);
            result.annotation = annotation;
            return result;
        };
    };

    template <typename T>
    function<Result<T>(vector<T>)>
    sepBy
    (function<Result<T>(vector<T>)> sep, function<Result<T>(vector<T>)> val=wildcard<T>(), string annotation="none")
    {
        return annotate(inOrder<T>({
        val,
        many<T>(inOrder<T>({sep, val}))
        }), annotation);
    };

    //All of these are pretty self explanatory, they check a Term to see if it is a particular group of characters
    const auto digits = singleTemplate<string>(is_digits);
    const auto alphas = singleTemplate<string>(is_alphas);
    const auto puncts = singleTemplate<string>(is_puncts);
    const auto uppers = singleTemplate<string>(is_uppers);
    const auto lowers = singleTemplate<string>(is_lowers);
}
