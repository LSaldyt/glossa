/// Copyright 2017 Lucas Saldyt
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
        auto terms  = seperate(sentence, lexmap.seperators, string_delimiters, comment_delimiter);
        auto tokens = Tokens(); 

        while (terms.size() > 0)
        {
            auto result = lexmap.identify(terms);
            tokens.push_back(get<0>(result));
            terms = get<1>(result);
        }

        return tokens;
    }

    LexMap buildLexMap(string lex_dir, vector<string> keywords)
    {
        vector<char> string_delimiters;
        string multiline_comment_delimiter;
        string comment_delimiter;

        vector<string> operators;
        vector<string> logicaloperators; 
        vector<string> punctuators; 
        vector<Seperator> whitespace; 

        // for language in inherits:
        concat(whitespace,        readWhitespaceFile(lex_dir + "whitespace"));
        concat(operators        , readFile(lex_dir + "operators"));
        concat(logicaloperators , readFile(lex_dir + "logicaloperators"));
        concat(punctuators      , readFile(lex_dir + "punctuators"));

        auto delims = readDelimiters(lex_dir);
        string_delimiters           = get<0>(delims);
        multiline_comment_delimiter = get<1>(delims);
        comment_delimiter           = get<2>(delims);

        LexMapTermSets term_sets = {
            make_tuple(keywords,         "keyword",         1), 
            make_tuple(logicaloperators, "logicaloperator", 3),
            make_tuple(operators,        "operator",        1),
            make_tuple(punctuators,      "punctuator",      3)};

        vector<LexMapLexer> lexer_set = {
            LexMapLexer(digits,            "int",    "literal",    3),
            LexMapLexer(doubles,           "double", "literal",    1),
            LexMapLexer(identifiers,       "*text*", "identifier", 3),
            LexMapLexer(startswith(comment_delimiter), "comment", "comment", 3)};

        for (auto delimiter : string_delimiters)
        {
            lexer_set.push_back(LexMapLexer(startswith(string(1, delimiter)), "string", "literal", 1));
        }
        return LexMap (term_sets, lexer_set, whitespace, string_delimiters, multiline_comment_delimiter, comment_delimiter);
    }

    /**
     * Function for reading in user-defined string and comment delimiters for a programming language
     */
    tuple<vector<char>, string, string> readDelimiters(string directory)
    {
        vector<char> string_delimiters;
        string multiline_comment_delimiter;
        string comment_delimiter;

        print("Reading delimiters from " + directory);
        auto string_delimiters_file  = readFile(directory + "string_delimiters");
        auto comment_delimiters_file = readFile(directory + "comment_delimiters");

        for (auto string_delimiter : string_delimiters_file)
        {
            assert(string_delimiter.size() == 1);
            string_delimiters.push_back(string_delimiter[0]);
        }

        if (comment_delimiters_file.size() == 2)
        {
            comment_delimiter           = comment_delimiters_file[0];
            multiline_comment_delimiter = comment_delimiters_file[1];
        }
        else
        {
            print("Warning: no comment delimiters read from " + directory);
        }
        return make_tuple(string_delimiters, multiline_comment_delimiter, comment_delimiter);
    }
}
