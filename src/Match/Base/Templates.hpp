#pragma once
#include "MatchResult.hpp"

namespace Match 
{
    template <typename T>
    std::function<Result<T>(std::vector<T>)>
    matchTemplate 
    (std::function<Consumed<T>(std::vector<T>)> consumer)
    {
        return [consumer](std::vector<T> terms)
            {
                auto result = Result<T>(false, std::vector<T>(), terms);

                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    std::vector<T> consumed = consumer_result.consumed;
                    std::vector<T> remaining (terms.begin() + consumed.size(), terms.end());
                    result = Result<T>(true, consumed, remaining);
                }
                return result;
            };
    }

    //using Comparator = std::function<bool(const std::string&)>;
    template <typename T>
    std::function<Result<T>(std::vector<T>)>
    singleTemplate 
    (std::function<bool(T)> comparator)
    {
        std::function<Consumed<T>(std::vector<T>)> consumer = [comparator](std::vector<T> terms)
        {
            Consumed<T> consumed(false, std::vector<T>()); //An empty list of terms, as nothing was yet consumed
            if (terms.size() > 0)
            {
                auto result = comparator(terms[0]);
                if (result)
                {
                    consumed = Consumed<T>(result, std::vector<T>(terms.begin(), terms.begin() + 1));
                }
            }
            return consumed;
        };
        return matchTemplate(consumer);
    }

    template <typename T>
    std::function<Result<T>(std::vector<T>)>
    multiTemplate(std::function<Consumed<T>(std::vector<T>)> consumer)
    {
        return matchTemplate([consumer](std::vector<T> terms)
        {
            auto consumed = std::vector<T>(); 
            while(terms.size() > 0)
            {
                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    consumed.insert(consumed.end(), consumer_result.consumed.begin(), consumer_result.consumed.end());
                    terms = std::vector<T>(terms.begin() + consumer_result.consumed.size(), terms.end());
                }
                else
                {
                    break;
                }
            }
            return Consumed<T>(true, consumed);
        });
    }
}
