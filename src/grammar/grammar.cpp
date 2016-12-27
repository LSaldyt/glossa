#include "grammar.hpp"

namespace grammar
{
shared_ptr<Symbol> annotateSymbol(shared_ptr<Symbol> s, string annotation)
{
    s->annotation = annotation;
    return s;
}

// Standard grammar constructor (From list of files)
Grammar::Grammar(vector<string> filenames, string directory) 
{
    for (auto filename : filenames)
    {
        grammar_map[filename] = read(directory + filename);
    }
}

vector<tuple<string, vector<vector<shared_ptr<Symbol>>>>> Grammar::identifyGroups(SymbolicTokens& tokens)
{
    vector<tuple<string, vector<vector<shared_ptr<Symbol>>>>> identified_groups;
    try 
    {
        // Consume all tokens
        while (tokens.size() > 0)
        {
            // Tag groups of tokens as certain language constructs
            auto result = identify(tokens);
            auto group  = toGroup(get<0>(result), get<1>(result));
            identified_groups.push_back(make_tuple(get<0>(result), group));
        }
    }
    catch (named_exception& e) 
    {
        print("Successfully identified:");
        for (auto identified_group : identified_groups)
        {
            print("Group identified as " + get<0>(identified_group));
            auto groups = get<1>(identified_group);
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    print(symbol->abstract());
                }
            }
        }
        print("Remaining:");
        for (auto token : tokens)
        {

        }
        throw;
    }

    return identified_groups;
}


vector<vector<shared_ptr<Symbol>>> Grammar::toGroup(string name, vector<Result<SymbolicToken>> results)
{
    auto construction_indices = get<1>(grammar_map[name]);

    vector<vector<shared_ptr<Symbol>>> groups;
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

// Helper function for reading in grammar files
//
// (Used after a keyword like anyOf or inOrder, which takes MULTIPLE parsers
SymbolicTokenParsers Grammar::readGrammarPairs(vector<string>& terms)
{
    SymbolicTokenParsers parsers;

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

// Convert a seperated line from a grammar file to a parser
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

// Master function for reading a grammar file
tuple<SymbolicTokenParsers, vector<int>> Grammar::read(string filename)
{
    SymbolicTokenParsers parsers;
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
                parsers.push_back(readGrammarTerms(terms));
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

// Lazily evaluate links between grammar files
SymbolicTokenParser Grammar::retrieveGrammar(string filename)
{
    return [filename, this](SymbolicTokens tokens)
    {
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
        SymbolicTokens tokens_copy(tokens);
        auto result = evaluateGrammar(parsers, tokens_copy);

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

// Identify a group of tokens from a larger set
tuple<string, vector<Result<SymbolicToken>>> 
Grammar::identify
(SymbolicTokens& tokens)
{
    SymbolicTokens tokens_copy(tokens);

    auto statement = grammar_map["statement"]; 
    auto parsers   = get<0>(statement);
    auto result    = evaluateGrammar(parsers, tokens_copy);

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

// Evaluate a list of parsers stored in the grammar_map
tuple<bool, vector<Result<SymbolicToken>>> 
Grammar::evaluateGrammar
(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
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


// Discard type information from SymbolicTokens
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

}
