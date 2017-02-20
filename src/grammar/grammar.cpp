#include "grammar.hpp"

namespace grammar
{
shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation)
{
    s->annotation = annotation;
    return s;
}

/// Standard grammar constructor (From list of files)
Grammar::Grammar(vector<string> filenames, string directory, string lex_dir) 
{
    readInherits(directory + "../../");
    for (auto filename : filenames)
    {
        print(filename);
        grammar_map[filename] = read(directory + filename);
    }
    readDelimiters(lex_dir);
    readLexRules(lex_dir);

    LexMapTermSets term_sets;
    term_sets.push_back(make_tuple(keywords,         "keyword",         1)); // Keywords are read in automatically from grammar file usage
    term_sets.push_back(make_tuple(logicaloperators, "logicaloperator", 3));
    term_sets.push_back(make_tuple(operators,        "operator",        1));
    term_sets.push_back(make_tuple(punctuators,      "punctuator",      3));

    vector<LexMapLexer> lexer_set = {
        LexMapLexer(just("    "s),     "tab",    "tab",        3),
        LexMapLexer(startswith("\t"s), "tab",    "tab",        3),
        LexMapLexer(digits,            "int",    "literal",    3),
        LexMapLexer(doubles,           "double", "literal",    1),
        LexMapLexer(identifiers,       "*text*", "identifier", 3)};

    if (not comment_delimiter.empty())
    {
        lexer_set.push_back(LexMapLexer(startswith(comment_delimiter), "comment", "comment", 3));
    }
    for (auto delimiter : string_delimiters)
    {
        lexer_set.push_back(LexMapLexer(startswith(string(1, delimiter)), "string", "literal", 1));
    }
    lexmap = LexMap (term_sets, lexer_set, whitespace);
}

void Grammar::readLexRules(string lex_dir)
{
    concat(whitespace,        readWhitespaceFile(lex_dir + "whitespace"));
    concat(operators        , readFile(lex_dir + "operators"));
    concat(logicaloperators , readFile(lex_dir + "logicaloperators"));
    concat(punctuators      , readFile(lex_dir + "punctuators"));
}

/**
 * High level function for identifying many syntatic constructs at once
 * @param tokens Vector of tokens to be identified
 * @param logger OutputManager to track verbose output
 * @return vector of annotated matrices, each representing a high level symbolic type (statement)
 */
IdentifiedGroups Grammar::identifyGroups(vector<SymbolicToken>& tokens, OutputManager logger)
{
    logger.log("Identifying groups with grammar");
    IdentifiedGroups identified_groups;
    try 
    {
        // Consume all tokens
        while (tokens.size() > 0)
        {
            // Tag groups of tokens as certain lexmap constructs
            logger.log("Attempting identification of remaining " + std::to_string(tokens.size()) + " tokens");
            auto result = identify(tokens, logger);
            logger.log("Identified group as " + get<0>(result) + ", grouping..");
            auto group  = toGroup(get<0>(result), get<1>(result));
            identified_groups.push_back(make_tuple(get<0>(result), group));
            logger.log("Group creation finished. " + std::to_string(tokens.size()) + " tokens remaining");
        }
    }
    catch (...) // Print the info we have so far, then re-raise any error 
    {
        logger.log("Successfully identified:");
        for (auto identified_group : identified_groups)
        {
            logger.log("Group identified as " + get<0>(identified_group));
            auto groups = get<1>(identified_group);
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    logger.log(symbol->abstract());
                }
            }
        }
        logger.log("Remaining:");
        for (auto token : tokens)
        {
            logger.log("Token (type: " + token.type + "), (subtype: " + token.sub_type + "), (text:" + token.text + ")");
        }
        throw;
    }
    logger.log("Group identification finished. " + std::to_string(identified_groups.size()) + " groups created");

    return identified_groups;
}


/**
 * Discards unwanted tokens marked by the user
 * @param name    Annotation for higher level syntactic construct (statement)
 * @param results Results of matching against a given statement
 * @return        2D matrix of Symbols
 */
SymbolMatrix Grammar::toGroup(string name, vector<Result<SymbolicToken>> results)
{
    auto construction_indices = get<1>(grammar_map[name]);

    SymbolMatrix groups;
    groups.push_back(vector<shared_ptr<Symbol>>());

    for (auto i : construction_indices)
    {
        // Account for the end-user's grouping instructions (grammar files)
        if (i == -1)
        {
            groups.push_back(vector<shared_ptr<Symbol>>());
        }
        else
        {
            auto result = results[i];
            result.consumed = clean(result.consumed); // Discard tokens that have been marked as unneeded

            auto grouped_tokens = reSeperate(result.consumed); // Expand multi-token parsers
            for (auto group : grouped_tokens)
            {
                for (auto t : group)
                {
                    groups.back().push_back(t.value);
                }
            }
        }
    }
    return groups;
}

/**
 * Helper function for reading in grammar files
 * (Used after a keyword like anyOf or inOrder, which takes MULTIPLE parsers
 * @param terms The terms of a line in a grammar file
 * @return Vector of symbolictokenparsers to be used by a higher-level parsing function
 */
vector<SymbolicTokenParser> Grammar::readGrammarPairs(vector<string>& terms)
{
    vector<SymbolicTokenParser> parsers;

    if (terms.size() % 2 != 0)
    {
        throw named_exception("Could not read type pairs"); // Need an even number of terms
    }
    for (int i = 0; i < (terms.size() / 2); i++)
    {
        int x = i * 2;
        vector<string> pair(terms.begin() + x, terms.begin() + x + 2);
        parsers.push_back(readGrammarTerms(pair));
    }

    return parsers;
}

/**
 * Converts a seperated line from a grammar file to a parser
 * Uses isomorphic functions to those in the match module (i.e. many, inOrder)
 * @param terms Whitespace seperated terms from a line in a grammar file
 * @return A parser that matches against the line described in the grammar file
 */
SymbolicTokenParser Grammar::readGrammarTerms(vector<string>& terms)
{
    SymbolicTokenParser parser;

    if (terms.size() == 2)
    {
        auto first = terms[0];
        bool keep  = true;
        // If first of pair starts with !, discard its parse result
        if (first[0] == '!')
        {
            first = string(first.begin() + 1, first.end());
            keep = false;
        }
        // Allow linking to other grammar files
        if (first == "link")
        {
            parser = retrieveGrammar(terms[1]);
        }
        // Parse by type only
        else if (terms[1] == "wildcard")
        {
            parser = typeParser(first);
        }
        // Parse by a specific subtype (ex "keyword return")
        else
        {
            if (first == "keyword")
            {
                keywords.push_back(terms[1]);
            }
            parser = dualTypeParser(first, terms[1]);
        }

        // Take care of a "!" if it was found early - make the parser discard its result
        if (not keep)
        {
            parser = discard(parser);
        }
    }
    else if (terms.size() > 2)
    {
        const auto keyword = terms[0];
        terms = vector<string>(terms.begin() + 1, terms.end());

        // Repeatedly parse a parser!
        if (keyword == "many")
        {
            parser = manySeperated(readGrammarTerms(terms)); 
        }
        // Require at least one success
        else if (keyword == "many1")
        {
            parser = manySeperated(readGrammarTerms(terms), true); 
        }
        // Optionally parse a parser
        else if (keyword == "optional")
        {
            parser = optional<SymbolicToken>(readGrammarTerms(terms));
        }
        // Run several parsers in order, failing if any of them fail
        else if (keyword == "inOrder")
        {
            parser = inOrder<SymbolicToken>(readGrammarPairs(terms));
        }
        // Choose from several parsers
        else if (keyword == "anyOf")
        {
            parser = anyOf<SymbolicToken>(readGrammarPairs(terms));
        }
        else
        {
            throw named_exception("Expected keyword, got: " + keyword);
        }
    }
    else
    {
        throw named_exception("Grammar file incorrectly formatted");
    }

    return parser;
}

/**
 * High level function for reading a grammar file
 * @param filename File to be read in
 * @return list of in order parsers, and a vector of construction indices
 * Construction indices indicate which parsed symbols are of significance
 */
tuple<vector<SymbolicTokenParser>, vector<int>> Grammar::read(string filename)
{
    vector<SymbolicTokenParser> parsers;
    auto content = readFile(filename);
    auto construct_line = content.back();
    content = slice(content, 0, -1);
    
    // Convert each line to a parser
    for (auto line : content)
    {
        if (line[0] != '#')
        {
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            if (not terms.empty())
            {
                try
                {
                    parsers.push_back(readGrammarTerms(terms));
                }
                catch (named_exception& e)
                {
                    print("Grammar file threw exception: " + filename);
                    throw;
                }
            }
        }
    }

    // The last line of a grammar file describes how to construct the syntax element
    vector<int> construct_indices;
    auto construct_terms = lex::seperate(construct_line, {make_tuple(" ", false)});
    for (auto t : construct_terms)
    {
        // Allow end user to specify how to group tokens
        if (t == "sep")
        {
            construct_indices.push_back(-1); // Signal for seperator
        }
        else
        {
            construct_indices.push_back(stoi(t));
        }
    }

    return make_tuple(parsers, construct_indices);
}

/**
 * Lazily evaluate links between grammar files
 * ex link expression allows grammar files to reference one another.
 * Because of laziness, circular references are allowed (as long as they terminate eventually)
 * @param filename File to be retrieved
 * @return Parser representing the syntax element that was retrieved
 */
SymbolicTokenParser Grammar::retrieveGrammar(string filename)
{
    return [filename, this](vector<SymbolicToken> tokens)
    {
        //print("Running parser for " + filename);
        std::vector<SymbolicTokenParser> parsers;

        // Retrieve a list of parsers from the grammar map
        auto search = grammar_map.find(filename);
        if (search != grammar_map.end())
        {
            parsers = std::get<0>(search->second);
        }
        else
        {
            throw named_exception(filename + " is not an element of the grammar map");
        }

        // Evaluate the parsers, preserving the tokens on failure
        vector<SymbolicToken> tokens_copy(tokens);
        auto result = evaluateGrammar(parsers, tokens_copy, OutputManager(0));
        if (get<0>(result))
        {
            auto group       = toGroup(filename, get<1>(result));
            auto constructed = make_shared<MultiSymbol>(MultiSymbol(filename, group));
            auto consumed    = vector<SymbolicToken>(1, SymbolicToken(constructed, filename, filename, ""));
            return Result<SymbolicToken>(true, consumed, tokens_copy); 
        }
        else
        {
            return Result<SymbolicToken>(false, {}, tokens);
        }
    };
}

/**
 * Identify a group of tokens from a larger set
 * Used repeatedly in the higher-level function identifyGroups
 * @param tokens Tokens to be identified
 * @param logger OutputManager for managing verbose output
 * @return Tuple of the form (annotation, results) where results are the collective match attempts against a particular (successful) syntax element
 */
tuple<string, vector<Result<SymbolicToken>>> 
Grammar::identify
(vector<SymbolicToken>& tokens, OutputManager logger)
{
    vector<SymbolicToken> tokens_copy(tokens);

    auto statement = grammar_map["statement"]; 
    auto parsers   = get<0>(statement);
    auto result    = evaluateGrammar(parsers, tokens_copy, logger);

    if (get<0>(result))
    {
        tokens = tokens_copy; // Apply our changes once we know the tokens were positively identified
        return make_tuple("statement", get<1>(result));
    }
    else
    {
        tokens_copy = tokens;
    }

    throw named_exception("Could not identify tokens");
}

/**
 * Evaluate a list of parsers stored in the grammar_map
 * @param parsers List of parsers from grammar_map
 * @param tokens List of tokens to be evaluated against
 * @param logger OutputManager for managing verbose output
 * @return Tuple of the form (result, results) where result is boolean, and results are Result<T> classes
 */
tuple<bool, vector<Result<SymbolicToken>>> 
Grammar::evaluateGrammar
(vector<SymbolicTokenParser> parsers, vector<SymbolicToken>& tokens, OutputManager logger)
{
    vector<Result<SymbolicToken>> results;

    int i = 0;
    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result);
        }
        else // Fail early if possible
        {
            return make_tuple(false, results);
        }
    }

    return make_tuple(true, results);
};


/**
 * Discard type information from vector<SymbolicToken>
 */
vector<shared_ptr<Symbol>> fromTokens(vector<SymbolicToken> tokens)
{
    vector<shared_ptr<Symbol>> symbols;
    symbols.reserve(tokens.size());

    for (auto t : tokens)
    {
        symbols.push_back(t.value); // t.type and t.sub_type are discarded
    }

    return symbols;
}

/**
 * Function for reading in user-defined string and comment delimiters for a programming language
 */
void Grammar::readDelimiters(string directory)
{
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
}

void Grammar::readInherits(string directory)
{
    print("Reading language inherits from " + directory);
    auto content = readFile(directory + "inherits");
    for (auto line : content)
    {
        //print(line);
	auto lex_dir = "languages/" + line + "/lex/";
        whitespace   = readWhitespaceFile(lex_dir + "whitespace"); //Override
        readDelimiters(lex_dir);
        readLexRules(lex_dir);

        auto inherit_dir = "languages/" + line + "/grammar/.__core__/";
        //print(inherit_dir);
        auto grammar_files = readFile(inherit_dir + "../core");
        for (auto filename : grammar_files)
        {
            //print(inherit_dir + filename);
            grammar_map[filename] = read(inherit_dir + filename);
        }
        // readInherits(inherit_dir); // Uncomment for heirarchical inheritance
    }
    print("Done reading inheritances for " + directory);
}

}
