#pragma once
#include "base/locale.hpp"
#include "base/templates.hpp"
#include "../types/result.hpp"

/**
 * Collection of common high-level functions used in parsing
 * Many of these mimic the parser-combinators in Haskell's Parsec library
 * These parsers are usually constructed ISOMORPHICALLY from grammar files provided by the user
 * For example parsing of function arguments might look like:
 *   optional link expression
 *   many inOrder !punctuator , link expression
 * Where "optional", "many" and "inOrder" are isomorphic to the high-level functions defined in this file
 * */

namespace match 
{
    /**
     * Run a list of Matcher<T>s sequentially, passing only if all of them pass.
     * Combines the result of all Matcher<T>s
     * Implicitly used in grammar files to run each line in order 
     * */
    template <typename T>
    Matcher<T> 
    inOrder 
    (vector<Matcher<T>> matchers)
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

    
    /**
     * Shorthand for matching by equality
     * just("x")({"x", "y"}) = Result(true, {"x"}, {"y"})
     * */
    template <typename T>
    Matcher<T>
    just
    (T value)
    {
        auto comparator = [value](T term){ return value == term; };
        return singleTemplate<T>(comparator);
    };

    /**
     * Specialized matcher for string matching
     * Used in lexing to identify strings and comments
     */
    const auto startswith = [](string value)
    {
        auto comparator = [value](string term){ 
            if (term.size() >= value.size())
            {
                return string(term.begin(), term.begin() + value.size()) == value; 
            }
            else
            {
                return false;
            }
        };
        return singleTemplate<string>(comparator);
    };

    /**
     * Optionally match a matcher. Never fails.
     * Example usage: beginning of function arguments (Multiple arguments aren't necessarily required)
     * */
    template <typename T>
    Matcher<T>
    optional
    (Matcher<T> matcher)
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

    /**
     * Pass only if a matcher fails
     * Equivalent to logical not operator
     * */
    template <typename T>
    Matcher<T>
    inverse
    (Matcher<T> matcher)
    {
        return [matcher](vector<T> terms)
        {
            auto result = matcher(terms);
            result.result = !result.result;
            return result;
        };
    }


    /** 
     * Attempt to parse any matcher from a list of matchers, failing only if all of the matchers fail, and passing if any of them pass
     */
    template <typename T>
    Matcher<T>
    anyOf 
    (vector<Matcher<T>> matchers)
    {
        return [matchers](vector<T> terms)
        {
            auto result = Result<T>(false, vector<T>(), terms);
            for (auto matcher : matchers)
            {
                auto match_result = matcher(terms);
                if ((match_result.result and match_result.consumed.size() > result.consumed.size()) 
                     or not result.result)
                {
                    result = match_result;
                }
            }
            return result;
        };
    };


    /**
     * Parse all matchers from a list of matchers, passing only if all of them pass
     * Consumes terms that the last matcher in the provided matchers would consume
     */
    template <typename T>
    Matcher<T>
    allOf
    (vector<Matcher<T>> matchers)
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

    /// Parse any term
    template <typename T>
    Matcher<T>
    wildcard
    ()
    {
        return singleTemplate<T>([](T t){return true;});
    }

    /**
     * Takes a matcher and parses it repeatedly, never fails
     * The nonempty option requires that the matcher be successful at least once
     */
    template <typename T>
    Matcher<T>
    many
    (Matcher<T> matcher, bool nonempty=false)
    {
        return [matcher, nonempty](vector<T> terms)
        {
            auto consumed = vector<T>(); 
            bool empty = true;

            while(terms.size() > 0)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    bool empty = false;
                    concat(consumed, result.consumed);
                    terms = slice(terms, result.consumed.size());
                }
                else
                {
                    break;
                }
            }

            if (nonempty)
            {
                return Result<T>(!empty, consumed, terms);
            }
            else
            {
                return Result<T>(true, consumed, terms);
            }
        };
    };

    /// Parse a single string against digits
    const auto digits = singleTemplate<string>(is_digits);
    /// Parse a single string against the alphabet 
    const auto alphas = singleTemplate<string>(is_alphas);
    /// Parse a single string against punctuator 
    const auto puncts = singleTemplate<string>(is_puncts);
    /// Parse a single string against uppercase letters 
    const auto uppers = singleTemplate<string>(is_uppers);
    /// Parse a single string against lowercase letters 
    const auto lowers = singleTemplate<string>(is_lowers);
    /// Parse a single string against valid identifier characters 
    const auto identifiers = singleTemplate<string>(is_identifiers);
    /// Parse a double
    const auto doubles = singleTemplate<string>(is_double);
}
