#include "lex.hpp"

namespace lex
{
    /** 
     * Converts a sentence to a vector of tokens based off of a defined lexmap
     * Also requires a set of string delimiters and a single inline comment delimiter
     * @param sentence The line to be converted into tokens
     * @param lexmap LexMap object containing lexing rules for a given language
     * @param string_delimiters Customized string delimiters for a language
     * @param comment_delimiter Customized inline comment delimiter for a langauge
     */
    Tokens lexWith(const string& sentence, const LexMap& lexmap, vector<char> string_delimiters, string comment_delimiter)
    {
        //print("Seperating sentence");
        // Seperate sentence..
        auto terms  = seperate(sentence, lexmap.seperators, string_delimiters, comment_delimiter);

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
            auto result = lexmap.identify(terms);
            tokens.push_back(get<0>(result));
            terms = get<1>(result);
        }

        return tokens;
    }
}
