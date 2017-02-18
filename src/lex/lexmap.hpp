#pragma once
#include "seperate.hpp"
#include "import.hpp"

namespace lex
{
    using LexMapTermSet  = tuple<vector<string>, string, int>;
    using LexMapTermSets = vector<LexMapTermSet>;

    /** 
     * A single definition for lexing a language element (ie int, operator)
     * Uses a matcher against a term to identify the given language element
     */
    struct LexMapLexer
    {
        Matcher<string> match;
        string name;
        string type;
        int precedence;
        LexMapLexer(Matcher<string> set_match_function, 
                      string set_name, 
                      string set_type, 
                      int set_precedence);
    };

    /**
     * A collection of lexers/term sets that can Identify terms
     * The set of single language element lexers is iterated over until a match is made
     * Otherwise, the last term remains unidentified and an error is thrown. 
     */
    struct LexMap
    {
        vector<Seperator>    seperators;
        bool          newline;

        LexMapTermSets language_term_sets;
        vector<LexMapLexer>   language_lexers;

        LexMap(const LexMapTermSets& set_term_sets,
                 const vector<LexMapLexer>&   set_language_lexers,
                 vector<Seperator> whitespace);
        LexMap();

        tuple<Token, vector<string>> identify(vector<string> terms) const;
    };
}
