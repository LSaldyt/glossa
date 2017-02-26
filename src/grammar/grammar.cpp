#include "grammar.hpp"

namespace grammar
{
shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation)
{
    s->annotation = annotation;
    return s;
}

/// Standard grammar constructor (From list of files)
Grammar::Grammar(string grammar_file, string lex_dir) 
{
    print("Creating grammar");
    readGrammarFile(grammar_file);
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
            auto ms_table = createMultiSymbolTable(get<0>(result), get<1>(result));
            identified_groups.push_back(make_tuple(get<0>(result), ms_table));
            logger.log("Group creation finished. " + std::to_string(tokens.size()) + " tokens remaining");
        }
    }
    catch (...) // Print the info we have so far, then re-raise any error 
    {
        logger.log("Successfully identified:");
        for (auto identified_group : identified_groups)
        {
            logger.log("Group identified as " + get<0>(identified_group));
            /*
            auto groups = get<1>(identified_group);
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    logger.log(symbol->abstract());
                }
            }
            */
        }
        logger.log("Failed on line: " + std::to_string(tokens[0].line));
        /*
        logger.log("Remaining:");
        for (auto token : tokens)
        {
            logger.log("Token (type: " + token.type + "), (subtype: " + token.sub_type + "), (text:" + token.text + ")");
        }
        */
        throw; // Very important
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

MultiSymbolTable Grammar::createMultiSymbolTable(string name, vector<Result<SymbolicToken>> results)
{
    auto index_tags = get<1>(grammar_map[name]);
    
    MultiSymbolTable ms_table;

    for (auto t : index_tags)
    {
        auto i = get<0>(t);
        auto tag = get<1>(t);
        auto result = results[i];
        result.consumed = clean(result.consumed); // Discard tokens that have been marked as unneeded

        auto grouped_tokens = reSeperate(result.consumed); // Expand multi-token parsers
        vector<shared_ptr<Symbol>> ms_group;
        for (auto group : grouped_tokens)
        {
            concat(ms_group, fromTokens(group));
        }
        ms_table[tag] = ms_group;
    }
    return ms_table;
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
        print("Running parser for " + filename);
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
            auto ms_table    = createMultiSymbolTable(filename, get<1>(result));
            auto constructed = make_shared<MultiSymbol>(MultiSymbol(filename, ms_table));
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

    assert(contains(grammar_map, "statement"));
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
        i++;
    }

    return make_tuple(true, results);
};

//using GrammarMap = unordered_map<string, tuple<vector<SymbolicTokenParser>, vector<int>>>; 
//funcdef: `keyword def` `$name$ identifier wildcard` `$args$ link parameters` `punctuator :` `$body$ many link statement` 
void Grammar::readGrammarFile(string filename)
{
    print("Reading grammar definitions from " + filename);
    auto content = readFile(filename);
    for (auto line : content)
    {
        print("Line: " + line);
        vector<SymbolicTokenParser> parsers;
        vector<tuple<int, string>> index_tags;
        auto terms = lex::seperate(line, {make_tuple("`", false)});
        assert(terms.size() > 1);
        auto tag = terms[0];
        replaceAll(tag, " ", "");
        replaceAll(tag, ":", "");
        terms = slice(terms, 1);
        int i = 0;
        for (auto t : terms)
        {
            if (not rtrim(t).empty())
            {
                auto interms = lex::seperate(t, {make_tuple(" ", false)});
                assert(interms.size() > 0);
                auto beginterm = interms[0];
                if (beginterm[0] == '@')
                {
                    replaceAll(beginterm, "@", "");
                    interms = slice(interms, 1);
                    parsers.push_back(readGrammarTerms(interms));
                    index_tags.push_back(make_tuple(i, beginterm));
                }
                else
                {
                    parsers.push_back(readGrammarTerms(interms));
                }
                i++;
            }
        }
        grammar_map[tag] = make_tuple(parsers, index_tags);
        print("Added " + tag + " to grammar map");
    }
}

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

void Grammar::readInherits(string directory)
{
    print("Reading language inherits from " + directory);
    /*
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
            //grammar_map[filename] = read(inherit_dir + filename);
        }
        // readInherits(inherit_dir); // Uncomment for heirarchical inheritance
    }
    */
    print("Done reading inheritances for " + directory);
}

}
