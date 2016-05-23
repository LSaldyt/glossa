#include "Templates.hpp"

namespace Parse
{
    ParseFunction parseTemplate(Consumer consumer)
    {
        ParseFunction func_template = [consumer](Tokens tokens)
            {
                auto result = Result(false, std::vector<std::string>(), tokens);

                auto consumer_result = consumer(tokens);
                if (consumer_result.result)
                {
                    Tokens parsed    = consumer_result.parsed;
                    Tokens remaining (tokens.begin() + parsed.size(), tokens.end());
                    result = Result(true, parsed, remaining);
                }
                return result;
            };
        return func_template;
    }

    ParseFunction singleTemplate(TokenComparator comparator)
    {
        Consumer consumer = [comparator](Tokens tokens)
        {
            Consumed consumed =  Consumed(false, Tokens()); //An empty list of tokens, as nothing was yet consumed
            if (tokens.size() > 0)
            {
                auto result = comparator(tokens[0]);
                if (result)
                {
                    consumed =  Consumed(result, Tokens(tokens.begin(), tokens.begin() + 1));
                }
            }
            return consumed;
        };
        return parseTemplate(consumer);
    }

    ParseFunction multiTemplate(Consumer consumer)
    {
        Consumer template_consumer = [consumer](Tokens tokens)
        {
            auto     parsed   = Tokens(); //An empty list of tokens
            while(tokens.size() > 0)
            {
                auto consumer_result = consumer(tokens);
                if (consumer_result.result)
                {
                    parsed.insert(parsed.end(), consumer_result.parsed.begin(), consumer_result.parsed.end());
                    tokens = Tokens(tokens.begin() + consumer_result.parsed.size(), tokens.end());
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
