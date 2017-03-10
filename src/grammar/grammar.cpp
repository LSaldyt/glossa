#include "grammar.hpp"

namespace grammar
{
shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation)
{
    s->annotation = annotation;
    return s;
}

/// Standard grammar constructor (From list of files)
Grammar::Grammar(string directory) 
{
    readInherits(directory + "inherits");

    auto content = readFile(directory + "grammar");
    for (auto line : content)
    {
        read(line);
    }
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
            auto ms_table = get<1>(identified_group);
            for (auto kv : ms_table)
            {
                for (auto symbol : kv.second)
                {
                    logger.log(symbol->abstract());
                }
            }
        }
        int line = tokens[0].line;
        logger.log("Failed on line: " + std::to_string(line));
        logger.log("Remaining:");
        string first = "";
        string second = "";
        for (auto token : tokens)
        {
            if (token.line == line)
            {
                first += token.text + " ";
            }
            else if (token.line == line + 1)
            {
                second += token.text + " ";
            }
            else
            {
                break;
            }
        }
        print(first + "(" + std::to_string(line) + ")", second + "(" + std::to_string(line + 1) + ")");
        throw; // Very important
    }
    logger.log("Group identification finished. " + std::to_string(identified_groups.size()) + " groups created");

    return identified_groups;
}

vector<string> Grammar::seperateGrammarLine(string line)
{
    vector<string> grammar_terms;

    auto terms = lex::seperate(line, {make_tuple("`", true)});
    bool in_escaped = false;
    for (auto t : terms)
    {
        if (t == "`")
        {
            in_escaped = !in_escaped;
        }
        else if (in_escaped)
        {
            grammar_terms.push_back(t);
        }
        else
        {
            auto interms = lex::seperate(t, {make_tuple(" ", false)});
            for (auto in_t : interms)
            {
                grammar_terms.push_back(in_t);
            }
        }
    }

    return std::move(grammar_terms);
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

    if (terms.size() == 1)
    {
        parsers.push_back(readGrammarTerms(terms));
        return parsers;
    }
    else if (terms.size() % 2 != 0)
    {
        string line = "";
        for (auto t : terms)
        {
            line += t + " ";
        }
        throw named_exception("Odd value of type pairs in grammar file: " + line); // Need an even number of terms
    }
    else
    for (int i = 0; i < (terms.size() / 2); i++)
    {
        int x = i * 2;
        vector<string> pair(terms.begin() + x, terms.begin() + x + 2);
        parsers.push_back(readGrammarTerms(pair));
    }

    return parsers;
}

/**
 * Parses parsers seperated by | tokens
 * @param terms Whitespace seperated terms from a line in a grammar file
 * @return A parser that matches against the line described in the grammar file
 */
vector<SymbolicTokenParser> Grammar::readAnyOf(vector<string>& terms)
{
    vector<vector<string>> term_groups;
    term_groups.push_back(vector<string>());
    for (auto t : terms)
    {
        if (t == "|")
        {
            term_groups.push_back(vector<string>());
        }
        else
        {
            term_groups.back().push_back(t);
        }
    }

    vector<SymbolicTokenParser> parsers;
    for (auto t_group : term_groups)
    {
        parsers.push_back(readGrammarTerms(t_group));
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

    assert(not terms.empty());

    auto keyword = terms[0];
    bool keep  = true;
    if (keyword[0] == '!')
    {
        keyword = sliceString(keyword, 1);
        keep = false;
    }
    if (terms.size() == 1) 
    {
        if (keyword[0] == '\'')
        {
            replaceAll(keyword, "'", "");
            parser = subTypeParser (keyword);
        }
        else
        {
            parser = retrieveGrammar(keyword);
        }
    }
    else 
    {
        auto sub_terms = slice(terms, 1);

        if (contains(terms, "|"s))
        {
            parser = anyOf(readAnyOf(terms));
        }
        // Repeatedly parse a parser!
        else if (keyword == "*" or keyword == "many")
        {
            parser = manySeperated(readGrammarTerms(sub_terms)); 
        }
        // Require at least one success
        else if (keyword == "many1")
        {
            parser = manySeperated(readGrammarTerms(sub_terms), true); 
        }
        // Optionally parse a parser
        else if (keyword == "optional")
        {
            parser = optional(readGrammarTerms(sub_terms));
        }
        // Choose from several parsers
        else if (keyword == "anyOf")
        {
            parser = anyOf(readGrammarPairs(sub_terms));
        }
        // Run several parsers in order, failing if any of them fail
        else if (keyword == "inOrder")
        {
            parser = inOrder(readGrammarPairs(sub_terms));
        }
        else if (keyword == "sep" or keyword == "sepKeep" or keyword == "sepWith" or keyword == "sepWithKeep")
        {
            vector<string> sub_parser_terms;
            vector<string> seperator_terms;
            if (keyword == "sepWith" or keyword == "sepWithKeep")
            {
                assert(sub_terms.size() > 2);
                seperator_terms = slice(sub_terms, 0, 2 - sub_terms.size());
                sub_parser_terms = slice(sub_terms, 2);
            }
            else
            {
                assert(sub_terms.size() > 1);
                seperator_terms.push_back(sub_terms[0]);
                sub_parser_terms = slice(sub_terms, 1);
            }
            auto seperator  = readGrammarTerms(seperator_terms);
            if (keyword == "sep" or keyword == "sepWith") seperator = discard(seperator);
            auto sub_parser = readGrammarTerms(sub_parser_terms); 
            parser = inOrder(vector<SymbolicTokenParser>{
                    sub_parser,
                    manySeperated(
                            inOrder(vector<SymbolicTokenParser>{
                                    seperator, 
                                    sub_parser})
                            )
                    });
        }
        else if (terms.size() == 2)
        {
            // Allow linking to other grammar files
            if (keyword == "link")
            {
                parser = retrieveGrammar(terms[1]);
            }
            // Parse by type only
            else if (terms[1] == "**" or terms[1] == "wildcard")
            {
                parser = typeParser(keyword);
            }
            // Parse by a specific subtype (ex "keyword return")
            else
            {
                if (keyword == "keyword")
                {
                    keywords.push_back(terms[1]);
                }
                parser = dualTypeParser(keyword, terms[1]);
            }
        }
        else
        {
            parser = inOrder<SymbolicToken>(readGrammarPairs(terms));
        }
    }
    if (not keep)
    {
        parser = discard(parser);
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
        std::vector<SymbolicTokenParser> parsers;

        string tag = filename;

        if (not contains(grammar_map, tag))
        {
            throw named_exception(tag+ " is not an element of the grammar map");
        }

        while(true)
        {
            if (not contains(grammar_map, tag)) break;

            parsers = get<0>(grammar_map[tag]);
            vector<SymbolicToken> tokens_copy(tokens);
            auto result = evaluateGrammar(parsers, tokens_copy, OutputManager(0));
            if (get<0>(result))
            {
                auto ms_table    = createMultiSymbolTable(filename, get<1>(result));
                auto constructed = make_shared<MultiSymbol>(MultiSymbol(filename, ms_table));
                auto consumed    = vector<SymbolicToken>(1, SymbolicToken(constructed, filename, filename, ""));
                return Result<SymbolicToken>(true, consumed, tokens_copy); 
            }
            tag += "_inherit";
        }
        return Result<SymbolicToken>(false, {}, tokens);
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

    string statement_tag = "statement";
    while (true)
    {
        if (not contains(grammar_map, statement_tag)) break;
        auto statement = grammar_map[statement_tag]; 
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
        statement_tag += "_inherit";
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

void Grammar::read(string line)
{
    auto terms = seperateGrammarLine(line);
    if (terms.empty()) return;
    auto tag = terms[0];
    replaceAll(tag, ":", "");
    print("Read grammar construct: " + tag);
    terms = slice(terms, 1);
    vector<SymbolicTokenParser> parsers;
    vector<tuple<int, string>> index_tags;
    int i = 0;
    for (auto t : terms)
    {
        auto interms = lex::seperate(t, {make_tuple(" ", false), make_tuple("**", true), make_tuple("*", true)});
        assert(not interms.empty());
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

    auto current = make_tuple(parsers, index_tags);
    decltype(current) temp; 
    auto nested_tag = tag;
    while (contains(grammar_map, nested_tag))
    {
        temp = grammar_map[nested_tag];
        grammar_map[nested_tag] = current;
        nested_tag += "_inherit";
        current = temp;
    }
    grammar_map[nested_tag] = current;
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

void Grammar::readInherits(string inherit_file)
{
    print("Reading language inherits from " + inherit_file);
    auto content = readFile(inherit_file);
    for (auto lang : content)
    {
        readInherits("languages/" + lang + "/inherits");
        for (auto line : readFile("languages/" + lang + "/grammar"))
        {
            read(line);
        }
    }
    print("Done reading language inherits");
}

}
