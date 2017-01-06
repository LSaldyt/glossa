#pragma once
#include "seperate.hpp"
#include "import.hpp"

namespace lex
{
    using LexMapTermSet  = tuple<vector<string>, string>;
    using LexMapTermSets = vector<LexMapTermSet>;

    // A single definition for lexing a language element (ie int, operator)
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

    // A collection of lexers/term sets that can Identify terms
    struct LexMap
    {
        Seperators    seperators;
        bool          newline;

        LexMapTermSets language_term_sets;
        vector<LexMapLexer>   language_lexers;

        LexMap(const LexMapTermSets& set_term_sets,
                 const vector<LexMapLexer>&   set_language_lexers,
                 Seperators whitespace);
        LexMap();

        tuple<Token, vector<string>> identify(vector<string> terms) const;
    };
}
