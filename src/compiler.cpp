#include "compiler.hpp"

/**
 * Demonstration of very high-level compiler usage
 */
int main(int argc, char* argv[])
{
    using namespace compiler;

    vector<string> args;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    assert(args.size() > 3);

    int verbosity = std::stoi(args[0]);

    string from = args[1];
    string to   = args[2];
    vector<string> files = slice(args, 3);

    compileFiles(files, "input", from, "output", to, verbosity);
    print("Compilation finished");
}

/**
 * Collection of high level functions for compilation
 */
namespace compiler
{

    /**
     * Read in simple symbol conversions from a file
     * i.e. append -> push_back 
     * @param filename File defining symbol conversions
     * @return Dictionary defining symbol conversions
     */
    unordered_map<string, string> readSymbolTable(string filename)
    {
        unordered_map<string, string> symbol_table;
        for (auto line : readFile(filename))
        {
            auto terms = lex::seperate(line, {make_tuple(" ", false)});
            assert(terms.size() == 3);
            symbol_table[terms[0]] = terms[2];
        }
        return symbol_table;
    }

    /**
     * High level function for creating a Grammar object from a set of grammar files
     * @param language Language to load
     * @return Grammar for parsing the given language
     */
    Grammar loadGrammar(string language)
    {
        print("Loading grammar for " + language);
	string lex_dir = "languages/" + language + "/lex/";
        string directory = "languages/" + language + "/grammar/";
        auto grammar_files = readFile(directory + "core");
        auto grammar       = Grammar(grammar_files, directory + ".__core__/", lex_dir);

        auto operators        = readFile(lex_dir + "operators");
        auto logicaloperators = readFile(lex_dir + "logicaloperators"); 
        auto punctuators      = readFile(lex_dir + "punctuators");

        LexMapTermSets term_sets;
        term_sets.push_back(make_tuple(grammar.keywords, "keyword",         1));         // Keywords are read in automatically from grammar file usage
        term_sets.push_back(make_tuple(logicaloperators, "logicaloperator", 3));
        term_sets.push_back(make_tuple(operators,        "operator",        1));
        term_sets.push_back(make_tuple(punctuators,      "punctuator",      3));

        vector<LexMapLexer> lexer_set = {
            LexMapLexer(just("    "s),     "tab",        "tab",        3),
            LexMapLexer(startswith("\t"s), "tab",        "tab",        3),
            LexMapLexer(digits,            "int",        "literal",    3),
            LexMapLexer(doubles,           "double",     "literal",    1),
            LexMapLexer(identifiers,       "*text*",    "identifier", 3)};

        lexer_set.push_back(LexMapLexer(startswith(grammar.comment_delimiter), "comment", "comment", 3));
        for (auto delimiter : grammar.string_delimiters)
        {
            lexer_set.push_back(LexMapLexer(startswith(string(1, delimiter)), "string", "literal", 1));
        }

        const vector<Seperator> whitespace = readWhitespaceFile(lex_dir + "whitespace");
        LexMap test_language(term_sets, lexer_set, whitespace);
        grammar.lexmap = test_language;
        return grammar;
    }

    /**
     * High level function for loading a code generator for a language
     * @param language Language for code generator to be loaded for
     * @return Generator which can construct source code for the given language
     */
    Generator loadGenerator(string language)
    {
        print("Loading constructors for " + language);
        auto constructor_files = readFile("languages/" + language + "/constructors/core");
        auto generator = Generator(constructor_files, "languages/" + language + "/constructors/");
        print("Done");
        return generator;
    }

    /**
     * High level function for loading a code transformer for a language
     * @param language Language for code generator to be loaded for
     * @return Transformer which can transformer AST for the given language
     */
    Transformer loadTransformer(string language)
    {
        print("Loading transformers for " + language);
        auto transformer_files = readFile("languages/" + language + "/transformers/core");
        auto transformer = Transformer(transformer_files, "languages/" + language + "/transformers/");
        print("Done");
        return transformer;
    }

    /**
     * High level function for transpilation
     * Converts source files of one language to source files of another, copying them into a new directory
     * @param filenames   List of files to transpile
     * @param input_dir   Input directory containing files in input language
     * @param input_lang  String name of input langauge
     * @param output_dir  Output directory that will contain files in output language
     * @param output_lang String name of output language
     * @param verbosity   Verbosity level of output
     */ 
    void compileFiles(vector<string> filenames, string input_dir, string input_lang, string output_dir, string output_lang, int verbosity)
    {
        auto grammar     = loadGrammar(input_lang);
        auto generator   = loadGenerator(output_lang);
        auto transformer = loadTransformer(input_lang);

        auto symbol_table = readSymbolTable("languages/symboltables/" + input_lang + output_lang);

        OutputManager logger(verbosity);

        for (auto& file : filenames)
        {
            compile(file, grammar, generator, transformer, symbol_table, input_dir, output_dir, logger);
        }
    }

    /**
     * Function for compilation of an individual file given a grammar, generator, symbol_table, input directory, and output directory
     * @param filename         File to be compiled
     * @param grammar          Grammar of input language
     * @param generator        Generator for output language
     * @param symbol_table     Dictionary of symbol conversions
     * @param input_directory  String of input directory
     * @param output_directory String name of output directory
     * @param logger           OutputManager class for managing verbose output. Use instead of print() calls
     */
    void compile(string filename, Grammar& grammar, Generator& generator,
                 Transformer& transformer,
                 unordered_map<string, string>& symbol_table, string input_directory, 
                 string output_directory, OutputManager logger)
    {
        logger.log("Reading file " + filename);
        auto content         = readFile     (input_directory + "/" + filename);
        logger.log("Lexing terms");
        auto tokens          = tokenPass    (content, grammar, symbol_table, logger); 
        logger.log("Creating symbols");
        auto symbolic_tokens = symbolicPass (tokens, logger);
        logger.log("Joining symbolic tokens");
        auto joined_tokens   = join         (symbolic_tokens, grammar.lexmap.newline);
        for(auto& jt : joined_tokens)
        {
            logger.log("Joined Token: " + jt.type + ", " + jt.sub_type + ", \"" + jt.text + "\"", 2);
        }
        logger.log("Identifying tokens from grammar:");
        auto identified_groups = grammar.identifyGroups(joined_tokens, logger);
        logger.log("Identified groups AST:");
        showAST(identified_groups, logger);
        transformer(identified_groups);
        showAST(identified_groups, logger);
        logger.log("Compiling identified groups");
        auto files = compileGroups(identified_groups, filename, generator, logger);

        logger.log("Initial file");
        for (auto line : content)
        {
            logger.log(line);
        }

        for (auto kv : files)
        {
            logger.log("Generated " + kv.first + " file:");
            auto body = get<0>(kv.second);
            auto path = get<1>(kv.second);
            for (auto line : body)
            {
                logger.log(line);
            }
            writeFile(body, output_directory + "/" + path);
        }
    }

    /**
     * Converts lines of source code to a list of tokens, provided a grammar
     * @param content Lines of source
     * @param grammar Grammar of input language
     * @param symbol_table Dictionary of symbol conversions
     * @return Vector of unsymbolized tokens (annotated terms)
     */
    std::vector<Tokens> tokenPass(std::vector<std::string> content, Grammar& grammar, unordered_map<string, string>& symbol_table, OutputManager logger)
    {
        std::vector<Tokens> tokens;
        string unseperated_content;
        for (auto line : content)
        {
            unseperated_content += line + "\n";
        }
        bool in_multiline_string = false;
        auto groups = lex::seperate(unseperated_content, {make_tuple(grammar.multiline_comment_delimiter, true)}, {}, "");
        for (auto group : groups)
        {
            if (group == grammar.multiline_comment_delimiter)
            {
                in_multiline_string = !in_multiline_string;
            }
            else if (in_multiline_string)
            {
                tokens.push_back(Tokens(1, Token(vector<string>(1, group), "comment", "comment")));
            }
            else
            {
                auto lines = lex::seperate(group, {make_tuple("\n", false)}, {}, "");
                for (auto it = lines.begin(); it != lines.end(); it++)
                {
                    string line = rtrim(*it); // Remove trailing whitespace
                    if (line.back() == '\\')
                    {
                        if (it + 1 != lines.end())
                        {
                            it++;
                            line = string(line.begin(), line.end() - 1) + " " + *it;
                        }
                    }
                    tokens.push_back(lexWith(line, grammar.lexmap, grammar.string_delimiters, grammar.comment_delimiter));
                }
            }
        }

        for (auto& token_group : tokens)
        {
            for (auto& token : token_group)
            {
                for (auto& value : token.values)
                {
                    logger.log("Token Value: " + value, 2);
                    if (contains(symbol_table, value))
                    {
                        value = symbol_table[value];
                    }
                }
            }
        }
        return tokens;
    }

    /**
     * Converts a vector of non-symbolic tokens to symbolic ones
     * @param tokens Non symbolized tokens
     * @return 2D array of Symbolized tokens
     */
    std::vector<vector<SymbolicToken>> symbolicPass(std::vector<Tokens> tokens, OutputManager logger)
    {
        std::vector<vector<SymbolicToken>> symbolic_tokens;
        for (auto token_group : tokens)
        {
            symbolic_tokens.push_back(toSymbolic(generatorMap, token_group, logger));
        }
        return symbolic_tokens;
    }

    /**
     * Converts a 2D matrix of tokens to a vector
     * @param token_groups 2D matrix of tokens
     * @param newline option to insert newlines between groups of tokens
     * @return Vector of symbolic tokens
     */
    vector<SymbolicToken> join(std::vector<vector<SymbolicToken>> token_groups, bool newline)
    {
        auto tokens = vector<SymbolicToken>();
        for (auto token_group : token_groups)
        {
            for (auto t : token_group)
            {
                tokens.push_back(t);
            }
            if (newline)
            {
                tokens.push_back(SymbolicToken(make_shared<Symbol>(Newline("\n")), "newline", "newline", "\n"));
            }
        }
        return tokens;
    }

    unordered_map<string, tuple<vector<string>, string>> compileGroups(vector<tuple<string, SymbolMatrix>> identified_groups,
                                                                       string filename,
                                                                       Generator &generator,
                                                                       OutputManager logger)
    {
        unordered_map<string, tuple<vector<string>, string>> files;
        for (auto identified_group : identified_groups)
        {
            logger.log("Compiling groups (Identified as " + get<0>(identified_group) + ")");
            string gen_with = "none";
            if (files.empty())
            {
                gen_with = filename;
            }
            unordered_set<string> names;
            logger.log("Generating code for " + get<0>(identified_group));
            auto a = getTime();
            auto generated = generator(names, get<1>(identified_group), get<0>(identified_group), gen_with, 1, logger);
            auto b = getTime();
            logger.log("Generation step took " + std::to_string((double)(b - a) / 1000000.) + "s");
            logger.log("Adding generated code to file content");
            for (auto fileinfo : generated)
            {
                string type         = get<0>(fileinfo);
                string path         = get<1>(fileinfo);
                vector<string> body = get<2>(fileinfo);

                if (contains(files, type))
                {
                    logger.log("Adding to " + type + " file");
                    concat(get<0>(files[type]), body);
                }
                else
                {
                    logger.log("Creating initial " + type + " file");
                    files[type] = make_tuple(body, path);
                }
            }
        }
        return files;
    }

    void showAST(const IdentifiedGroups& identified_groups, OutputManager logger)
    {
        for (const auto& identified_group : identified_groups)
        {
            auto groups = get<1>(identified_group);
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    auto abstract = symbol->abstract();
                    logger.log(abstract);
                }
            }
        }
    }
}
