#include "lex.hpp"

namespace lex
{
    // Convert a sentence to a vector of tokens based off of a defined language
    Tokens lexWith(const vector<string>& sentence, const Language& language, vector<char> vector<string>_delimiters, vector<string> comment_delimiter)
    {
        //const vector<string>& cleanline = removeComments(sentence);
        //print("Seperating sentence");
        // Seperate sentence..
        auto terms  = seperate(sentence, language.seperators, vector<string>_delimiters, comment_delimiter);

        //print("Seperated terms:");
        for (auto t : terms)
        {
            //print(t);
        }
        //print("Done"); 

        auto tokens = Tokens(); // Empty vector of tokens

        // Do the actual identification here
        while (terms.size() > 0)
        {
            auto result = language.identify(terms);
            tokens.push_back(get<0>(result));
            terms = get<1>(result);
        }

        return tokens;
    }
}
