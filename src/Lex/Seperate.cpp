#include <algorithm>
#include <tuple>
#include "Seperate.hpp"
#include <iostream>

namespace Lex
{
    std::tuple<bool, bool, int> find_seperator(std::string s, const Seperators &seperators)
    {
        auto found = std::make_tuple(false, false, 0);
        for (auto seperator : seperators)
        {
            auto seperator_string = std::get<0>(seperator);
            if (seperator_string.size() < s.size())
            {
                bool exited_early = false;
                for (unsigned i = 0; i < seperator_string.size(); i++)
                {
                    if (seperator_string[i] != s[i])
                    {
                        exited_early = true;
                        break;
                    }
                }

                if (!exited_early)
                {
                    found = std::make_tuple(true, std::get<1>(seperator), seperator_string.size());
                    break;
                }
            }
            else
            {
                std::cout << "Seperator string is larger than term" << std::endl;
                std::cout << "-" << seperator_string << "-, -" << s << "-" << std::endl;
            }
        }
        return found;
    }

    Terms seperate(const std::string& sentence, const Seperators &seperators)
    {
        auto terms = Terms();
        auto current = sentence.begin();

        for(auto it = sentence.begin(); it != sentence.end(); ++it)
        {
            auto found = find_seperator(std::string(it, sentence.end()), seperators);
            if(std::get<0>(found))
            {
                if (current != it)
                {
                    terms.push_back(std::string(current, it));
                }
                if(std::get<1>(found))
                {
                    terms.push_back(std::string(it, it+std::get<2>(found)));
                }
                current = it + std::get<2>(found);
                if (std::get<2>(found) > 0)
                {
                    it += std::get<2>(found) - 1;
                }
            }
            else if(it+1 == sentence.end())
            {
                terms.push_back(std::string(current, it+1));
            }
        }
        return terms;
    }
}
