#include "Parse.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <algorithm>

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

    ParseFunction singleTemplate(TokenComparator comparator)
    {
        ParseFunction func_template = [comparator](Tokens tokens)
            {
                auto result = Result(false, std::vector<std::string>(), tokens);
                if (tokens.size() > 0)
                {
                    if (comparator(tokens[0]))
                    {
                        Tokens parsed    (tokens.begin(), tokens.begin() + 1);
                        Tokens remaining (tokens.begin() + 1, tokens.end());
                        result = Result(true, parsed, remaining);
                    }
                }
                return result;
            };
        return func_template;
    }

    ParseFunction just(std::string value)
    {
        auto comparator = [value](std::string token){ return value == token; };
        return singleTemplate(comparator);
    }
}
//Actual parsers


//
// auto  just(std::string token)
// {
//     auto func_template = lambda[](std::vector<std::string> tokens)
//     {
//         if (tokens.size() > 0)
//         {
//             if (tokens[0] == token)
//             {
//                 std::vector<std::string> parsed    (tokens.begin(), tokens.begin() + 1);
//                 std::vector<std::string> remaining (tokens.begin() + 1, tokens.end());
//                 return Result(true, parsed, remaining);
//             }
//         }
//         return Result(false, std::vector<std::string>(), tokens);
//     }
//     return func_template
// }
