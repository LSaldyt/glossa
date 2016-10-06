#include "seperate.hpp"

namespace Lex
{
    // Given a remaining string, find the first seperator present in the beginning section
    // Return (success, keep_seperator, len_seperator)
    tuple<bool, bool, int> find_seperator(string s, const Seperators &seperators)
    {
        // Default value (failure)
        auto found = make_tuple(false, false, 0);
        for (auto seperator : seperators)
        {
            auto seperator_string = get<0>(seperator);
            // Match two strings, character by character
            if (seperator_string.size() <= s.size())
            {
                bool exited_early = false;
                for (unsigned i = 0; i < seperator_string.size(); i++)
                {
                    if (seperator_string[i] != s[i])
                    {
                        // Either try the next seperator or (if none are left) exit failure
                        exited_early = true;
                        break;
                    }
                }

                // If we matched an entire seperator, return success
                if (!exited_early)
                {
                    found = make_tuple(true, get<1>(seperator), seperator_string.size());
                    break;
                }
            }
        }
        return found;
    }

    Terms seperate(const string& sentence, const Seperators &seperators)
    {
        auto terms   = Terms();
        auto current = sentence.begin();

        // A more complicated algorithm:
        // retains two iterators along the sentence (current, it)
        // advance the it iterator until a seperator is found,
        // then push from current to it into terms
        // set current to it
        // push the seperator into terms if it should be kept (bool second tuple field)
        // repeat until no sentence is left, push remaining sentence into terms
        // (Seperate a sentence into terms in <O(n^2)? time)
        for(auto it = sentence.begin(); it != sentence.end(); ++it)
        {
            // Special case for strings (save some work)
            if (*it == '"')
            {
                // remove the string between two quotemarks and push into terms
                string remaining(it + 1, sentence.end());
                size_t found = remaining.find("\"");
                if (found!=string::npos) // IF there actually is a second quotation mark
                {
                    found += 2; //Account for " characters surrounding the string
                    terms.push_back(string(it, it + found));
                    current = it + found;
                    it      = it + found;
                }
            }
            // Normal seperation
            auto found = find_seperator(string(it, sentence.end()), seperators);
            if(get<0>(found)) // Beginning of the remaining sentence is a seperator
            {
                // If there is a term we have seperated, add it to terms
                if (current != it)
                {
                    terms.push_back(string(current, it));
                }
                // Keep the seperator, if applicable
                if(get<1>(found))
                {
                    terms.push_back(string(it, it+get<2>(found)));
                }
                // Update current to be it + len(seperator)
                current = it + get<2>(found);
                if (get<2>(found) > 0)
                {
                    it += get<2>(found) - 1;
                }
            }
            // If there is no sentence left to seperate, push it to terms
            else if(it+1 == sentence.end())
            {
                terms.push_back(string(current, it+1));
            }
        }
        return terms;
    }
}
