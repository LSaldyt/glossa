#include "Templates.hpp"

namespace Parse
{
    ParseFunction parseTemplate(Consumer consumer)
    {
        ParseFunction func_template = [consumer](Terms terms)
            {
                auto result = Result(false, Terms(), terms);

                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    Terms parsed    = consumer_result.parsed;
                    Terms remaining (terms.begin() + parsed.size(), terms.end());
                    result = Result(true, parsed, remaining);
                }
                return result;
            };
        return func_template;
    }

    ParseFunction singleTemplate(Comparator comparator)
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
        return parseTemplate(consumer);
    }

    ParseFunction multiTemplate(Consumer consumer)
    {
        Consumer template_consumer = [consumer](Terms terms)
        {
            auto parsed = Terms(); //An empty list of terms
            while(terms.size() > 0)
            {
                auto consumer_result = consumer(terms);
                if (consumer_result.result)
                {
                    parsed.insert(parsed.end(), consumer_result.parsed.begin(), consumer_result.parsed.end());
                    terms = Terms(terms.begin() + consumer_result.parsed.size(), terms.end());
                }
                else
                {
                    break;
                }
            }
            return Consumed(true, parsed);
        };
        return parseTemplate(template_consumer);
    }
}
