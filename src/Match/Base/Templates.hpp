#pragma once
#include "Import.hpp"
#include "MatchResult.hpp"

namespace Match 
{
    template <typename T>
    function<Result<T>(vector<T>)>
    matchTemplate 
    (function<Consumed<T>(vector<T>)> consumer)
    {
        return [consumer](vector<T> terms)
            {
                auto result = Result<T>(false, vector<T>(), terms);

                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    vector<T> consumed = consumer_result.consumed;
                    vector<T> remaining (terms.begin() + consumed.size(), terms.end());
                    result = Result<T>(true, consumed, remaining);
                }
                return result;
            };
    }

    //using Comparator = function<bool(const string&)>;
    template <typename T>
    function<Result<T>(vector<T>)>
    singleTemplate 
    (function<bool(T)> comparator)
    {
        function<Consumed<T>(vector<T>)> consumer = [comparator](vector<T> terms)
        {
            Consumed<T> consumed(false, vector<T>()); //An empty list of terms, as nothing was yet consumed
            if (terms.size() > 0)
            {
                auto result = comparator(terms[0]);
                if (result)
                {
                    consumed = Consumed<T>(result, vector<T>(terms.begin(), terms.begin() + 1));
                }
            }
            return consumed;
        };
        return matchTemplate<T>(consumer);
    }

    template <typename T>
    function<Result<T>(vector<T>)>
    multiTemplate
    (function<Consumed<T>(vector<T>)> consumer)
    {
        return matchTemplate<T>([consumer](vector<T> terms)
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
