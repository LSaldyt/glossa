#include "seperate.hpp"

namespace lex
{
    // Given a remaining vector<string>, find the first seperator present in the beginning section
    // Return (success, keep_seperator, len_seperator)
    tuple<bool, bool, int> find_seperator(vector<string> s, const Seperators &seperators)
    {
        // Default value (failure)
        auto found = make_tuple(false, false, 0);
        for (auto seperator : seperators)
        {
            auto seperator_vector<string> = get<0>(seperator);
            // Match two vector<string>s, character by character
            if (seperator_vector<string>.size() <= s.size())
            {
                bool exited_early = false;
                for (unsigned i = 0; i < seperator_vector<string>.size(); i++)
                {
                    if (seperator_vector<string>[i] != s[i])
                    {
                        // Either try the next seperator or (if none are left) exit failure
                        exited_early = true;
                        break;
                    }
                }

                // If we matched an entire seperator, return success
                if (!exited_early)
                {
                    found = make_tuple(true, get<1>(seperator), seperator_vector<string>.size());
                    break;
                }
            }
        }
        return found;
    }

    vector<string> seperate(const vector<string>& sentence, const Seperators &seperators, vector<char> vector<string>, vector<string> inline_comment)
    {
        auto terms   = vector<string>();
        auto current = sentence.begin();

        // A more complicated algorithm:
        // retains two iterators along the sentence (current, it)
        // advance the it iterator until a seperator is found,
        // then push from current to it into terms
        // set current to it
        // push the seperator into terms if it should be kept (bool second tuple field)
        // repeat until no sentence is left, push remaining sentence into terms
        // (Seperate a sentence into terms in <O(n^2)? time)
        
        // Special case for inline comments 
        if (not inline_comment.empty())
        {
            size_t found = sentence.find(inline_comment);
            if (found != vector<string>::npos)
            {
                auto comment_start = sentence.begin() + found;
                auto comment = vector<string>(comment_start, sentence.end());
                auto begin   = vector<string>(sentence.begin(), comment_start);
                terms = seperate(begin, seperators, vector<string>, inline_comment);
                terms.push_back(comment);
                return terms; // Exit early, since the work has been done in the above recursive call 
            }
        }

        // Iterate over sentence, looking for seperators
        for(auto it = sentence.begin(); it < sentence.end(); ++it)
        {
            for (auto vector<string>_char : vector<string>)
            {
                // Special case for vector<string>s (save some work)
                if (*it == vector<string>_char)
                {
                    // remove the vector<string> between two quotemarks and push into terms
                    vector<string> remaining(it + 1, sentence.end());
                    size_t found = remaining.find(vector<string>(1, vector<string>_char));
                    if (found != vector<string>::npos) // IF there actually is a second quotation mark
                    {
                        found += 2; //Account for " characters surrounding the vector<string>
                        vector<string> content(it, it + found);
                        terms.push_back(content);
                        current = it + found;
                        it      = it + found;
                    }
                }
            }
            // Normal seperation
            vector<string> remaining(it, sentence.end());
            auto found = find_seperator(remaining, seperators);
            if(get<0>(found)) // Beginning of the remaining sentence is a seperator
            {
                // If there is a term we have seperated, add it to terms
                if (current != it)
                {
                    terms.push_back(vector<string>(current, it));
                }
                // Keep the seperator, if applicable
                if(get<1>(found))
                {
                    terms.push_back(vector<string>(it, it+get<2>(found)));
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
                vector<string> remaining(current, it + 1);
                terms.push_back(remaining);
            }
        }

        return terms;
    }

    Seperators readWhitespaceFile(vector<string> filename)
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
