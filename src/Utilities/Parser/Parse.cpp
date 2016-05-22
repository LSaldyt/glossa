#include "Parse.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <tuple>

namespace Parse
{

    std::string strip_punctuation(const std::string& sentence)
    {
        std::string result;
        std::remove_copy_if(sentence.begin(), sentence.end(),
                                std::back_inserter(result), //Store output
                                std::ptr_fun<int, int>(&std::ispunct)
                               );
        return result;
    }

    std::tuple<bool, bool> find_seperator(char c, const Seperators &seperators)
    {
        auto found = std::make_tuple(false, false);
        for (auto seperator : seperators)
        {
            if (std::get<0>(seperator) == c)
            {
                found = std::make_tuple(true, std::get<1>(seperator));
                break;
            }
        }
        return found;
    }

    Tokens seperate(const std::string& sentence, const Seperators &seperators)
    {
        auto tokens = Tokens();
        auto current = sentence.begin();

        for(auto it = sentence.begin(); it != sentence.end(); ++it)
        {
            auto found = find_seperator(*it, seperators);
            if(std::get<0>(found))
            {
                tokens.push_back(std::string(current, it));
                if(std::get<1>(found))
                {
                    tokens.push_back(std::string(1, *it));
                }
                current = it+1;
            }
        }
        return tokens;
    }

    Tokens tokenize(const std::string& sentence)
    {
        std::string str = strip_punctuation(sentence);

        // construct a stream from the string
        std::stringstream strstr(str);

        // use stream iterators to copy the stream to the vector as whitespace separated strings
        std::istream_iterator<std::string> it(strstr);
        std::istream_iterator<std::string> end;
        Tokens results(it, end);

        return results;
    };

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

    ParseFunction many(ParseFunction parser)
    {
        Consumer consumer = [parser](Tokens tokens)
        {
            auto result = parser(tokens);
            return Consumed(result.result, result.parsed);
        };
        return multiTemplate(consumer);
    }

    ParseFunction just(std::string value)
    {
        auto comparator = [value](std::string token){ return value == token; };
        return singleTemplate(comparator);
    }

}
