#include "seperate.hpp"

namespace lex
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

    Terms seperate(const string& sentence, const Seperators &seperators, bool strings)
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
        for(auto it = sentence.begin(); it < sentence.end(); ++it)
        {
            if (strings)
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
                        string content(it, it + found);
                        terms.push_back(content);
                        current = it + found;
                        it      = it + found;
                    }
                }
            }
            // Normal seperation
            string remaining(it, sentence.end());
            auto found = find_seperator(remaining, seperators);
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
                    it += (get<2>(found) - 1);
                }
            }
            // If there is no sentence left to seperate, push it to terms
            else if(it + 1 == sentence.end())
            {
                string remaining(current, it + 1);
                terms.push_back(remaining);
            }
        }

        return terms;
    }

    Seperators readWhitespaceFile(string filename)
    {
        Seperators whitespace;
        auto content = readFile(filename);
        for (auto line : content)
        {
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            assert(terms.size() == 2);
            auto keyword = terms[0];
            bool keep    = terms[1] == "true";
            
            if (keyword == "indent")
            {
                whitespace.push_back(make_tuple("\t",   keep));
                whitespace.push_back(make_tuple("    ", keep));
            }
            else if (keyword == "space")
            {
                whitespace.push_back(make_tuple(" ", keep));
            }
            else if (keyword == "newline")
            {
                whitespace.push_back(make_tuple("\n", keep));
            }
            else
            {
                print("Unrecognized whitespace keyword: " + keyword);
            }
        }
        return whitespace;
    }
}
