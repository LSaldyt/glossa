/// Copyright 2017 Lucas Saldyt
#pragma once
#include "import.hpp"
#include "matchresult.hpp"

/**
 * Series of template functions for producing Matcher<T> functions from lower-level functions
 *
 */

namespace match 
{
    // Return a match result object from a parse attempt of a vector of any type T
    template <typename T>
    using Matcher = function<Result<T>(vector<T>)>;
    // Return a consumption result object from a parse attempt of a vector of any type T
    template <typename T>
    using Consumer = function<Consumed<T>(vector<T>)>;
    /**
     * Meta-function for producing a matcher from a consumer
     * Tracks consumed terms, remaining terms, and match result
     * */
    template <typename T>
    Matcher<T>
    matchTemplate 
    (Consumer<T> consumer)
    {
        return [consumer](vector<T> terms)
            {
                auto result = Result<T>(false, vector<T>(), terms);

                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    vector<T> consumed = consumer_result.consumed;
                    // Build set of remaining terms, which wasn't available in the consumption result
                    vector<T> remaining (terms.begin() + consumed.size(), terms.end());
                    result = Result<T>(true, consumed, remaining);
                }
                return result;
            };
    }

    /**
     * Creates a Matcher<T> object that compares a single type T against a predicate
     * If the predicate passes, the first type T in the vector will be matched against and consumed
     */
    template <typename T>
    Matcher<T>
    singleTemplate 
    (function<bool(T)> comparator)
    {
        Consumer<T> consumer = [comparator](vector<T> terms)
        {
            Consumed<T> consumed(false, vector<T>()); //An empty list of terms, as nothing was yet consumed
            if (terms.size() > 0)
            {
                auto result = comparator(terms[0]); // Test predicate
                if (result)
                {
                    consumed = Consumed<T>(result, vector<T>(terms.begin(), terms.begin() + 1));
                }
            }
            return consumed;
        };
        return matchTemplate<T>(consumer); // Convert Consumer<T> to Matcher<T>
    }

    /**
     * Builds a Matcher<T> that repeatedly runs a consumer against a vector of terms until the consumer fails 
     *   or there are no more terms to match against.
     * */
    template <typename T>
    Matcher<T>
    multiTemplate
    (Consumer<T> consumer)
    {
        return matchTemplate<T>([consumer](vector<T> terms) // Convert a Consumer<T> to a Matcher<T>
        {
            string annotation = "none";
            auto consumed = vector<T>(); 
            while(terms.size() > 0)
            {
                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    annotation = consumer_result.annotation;
                    consumed.insert(consumed.end(), consumer_result.consumed.begin(), consumer_result.consumed.end());
                    terms = vector<T>(terms.begin() + consumer_result.consumed.size(), terms.end());
                }
                else
                {
                    break;
                }
            }
            return Consumed<T>(true, consumed, annotation);
        });
    }
}
