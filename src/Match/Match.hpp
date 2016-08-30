#pragma once
#include "Base/Locale.hpp"
#include "Base/Templates.hpp"
#include "../Types/Result.hpp"

namespace Match 
{

    template <typename T>
    Result<T> inOrder (std::vector<std::function<Result<T>(std::vector<T>)>> matchers)
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

    const auto just = [](auto value)
    {
        auto comparator = [value](decltype(value) term){ return value == term; };
        return singleTemplate<decltype(value)>(comparator);
    };

    const auto startswith = [](std::string value)
    {
        auto comparator = [value](Term term){ 
            if (term.size() >= value.size())
            {
                return std::string(term.begin(), term.begin() + value.size()) == value; 
            }
            else
            {
                return false;
            }
        };
        return singleTemplate<Term>(comparator);
    };

    template <typename T>
    Result<T>
    inverse
    (std::function<Result<T>(std::vector<T>)> matcher)
    {
        return matchTemplate<T>([matcher](std::vector<T> terms)
        {
            auto result = matcher(terms);
            result.result = !result.result;
            return result;
        });
    }

    // Attempt to parse any matcher from a list of matchers, failing only if all of the matchers fail, and passing if any of them pass
    template <typename T>
    Result<T>
    anyOf 
    (std::vector<std::function<Result<T>(std::vector<T>)>> matchers)
    {
        return matchTemplate<T>([matchers](std::vector<T> terms)
        {
            auto result = Result<T>(false, std::vector<T>(), terms);
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
        });
    };


    //Parse all matchers from a list of matchers, passing only if all of them pass
    template <typename T>
    Result<T>
    allOf
    (std::vector<std::function<Result<T>(std::vector<T>)>> matchers)
    {
        return matchTemplate<T>([matchers](std::vector<T> terms)
        {
            auto result = Result<T>(false, std::vector<T>(), terms);
            for (auto matcher : matchers)
            {
                auto match_result = matcher(terms);
                if(!match_result.result)
                {
                    result = Result<T>(false, std::vector<T>(), terms);
                    break;
                }
                else
                {
                    result = match_result;
                }
            }
            return result;
        });
    };

    // Parse any term
    template <typename T>
    std::function<Result<T>(std::vector<T>)>
    wildcard
    ()
    {
        return singleTemplate<T>([](T t){return true;});
    }

    // Takes a matcher and parses it repeatedly, never fails
    template <typename T>
    Result<T>
    many
    (std::function<Result<T>(std::vector<T>)> matcher)
    {
        return multiTemplate([matcher](std::vector<T> terms)
        {
            auto result = matcher(terms);
            return Consumed<T>(result.result, result.consumed);
        });
    };

    template <typename T>
    Result<T>
    sepBy
    (std::function<Result<T>(std::vector<T>)> sep, std::function<Result<T>(std::vector<T>)> val=wildcard<T>())
    {
        return inOrder<T>({
        val,
        many<T>(inOrder<T>({sep, val}))
        });
    };

    //All of these are pretty self explanatory, they check a Term to see if it is a particular group of characters
    const auto digits = singleTemplate<std::string>(is_digits);
    const auto alphas = singleTemplate<std::string>(is_alphas);
    const auto puncts = singleTemplate<std::string>(is_puncts);
    const auto uppers = singleTemplate<std::string>(is_uppers);
    const auto lowers = singleTemplate<std::string>(is_lowers);
}
