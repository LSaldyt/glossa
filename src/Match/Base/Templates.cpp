#include "Templates.hpp"

namespace Match 
{
    MatchFunction matchTemplate(Consumer consumer)
    {
        MatchFunction func_template = [consumer](Terms terms)
            {
                auto result = Result(false, Terms(), terms);

                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    Terms consumed    = consumer_result.consumed;
                    Terms remaining (terms.begin() + consumed.size(), terms.end());
                    result = Result(true, consumed, remaining);
                }
                return result;
            };
        return func_template;
    }

    MatchFunction singleTemplate(Comparator comparator)
    {
        Consumer consumer = [comparator](Terms terms)
        {
            Consumed consumed =  Consumed(false, Terms()); //An empty list of terms, as nothing was yet consumed
            if (terms.size() > 0)
            {
                auto result = comparator(terms[0]);
                if (result)
                {
                    consumed =  Consumed(result, Terms(terms.begin(), terms.begin() + 1));
                }
            }
            return consumed;
        };
        return matchTemplate(consumer);
    }

    MatchFunction multiTemplate(Consumer consumer)
    {
        Consumer template_consumer = [consumer](Terms terms)
        {
            auto consumed = Terms(); //An empty list of terms
            while(terms.size() > 0)
            {
                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    consumed.insert(consumed.end(), consumer_result.consumed.begin(), consumer_result.consumed.end());
                    terms = Terms(terms.begin() + consumer_result.consumed.size(), terms.end());
                }
                else
                {
                    break;
                }
            }
            return Consumed(true, consumed);
        };
        return matchTemplate(template_consumer);
    }
}
