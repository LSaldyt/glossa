#include <algorithm>
#include <tuple>
#include "Seperate.hpp"

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
                if(current != it)
                {
                    tokens.push_back(std::string(current, it));
                }
                if(std::get<1>(found))
                {
                    tokens.push_back(std::string(1, *it));
                }
                current = it+1;
            }
            else if(it+1 == sentence.end())
            {
                tokens.push_back(std::string(current, it+1));
            }
        }
        return tokens;
    }
}
