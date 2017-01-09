#include "compiler.hpp"

/**
 * Demonstration of very high-level compiler usage
 */
int main(int argc, char* argv[])
{
    using namespace compiler;

    vector<string> args;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            args.push_back(argv[i]);
        }
    }

    assert(args.size() > 2);

    string from = args[0];
    string to   = args[1];
    vector<string> files = slice(args, 2);

    compileFiles(files, "input", from, "output", to);
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
        string directory = "languages/" + language + "/grammar/";
        auto grammar_files = readFile(directory + "core");
        auto grammar       = Grammar(grammar_files, directory);

        auto operators        = readFile(directory + "operators");
        auto logicaloperators = readFile(directory + "logicaloperators"); 
        auto punctuators      = readFile(directory + "punctuators");

        LexMapTermSets term_sets;
        term_sets.push_back(make_tuple(grammar.keywords, "keyword"));         // Keywords are read in automatically from grammar file usage
        term_sets.push_back(make_tuple(logicaloperators, "logicaloperator"));
        term_sets.push_back(make_tuple(operators,        "operator"));
        term_sets.push_back(make_tuple(punctuators,      "punctuator"));

        vector<LexMapLexer> lexer_set = {
            LexMapLexer(just("    "s),     "tab",        "tab",        3),
            LexMapLexer(startswith("\t"s), "tab",        "tab",        3),
            LexMapLexer(digits,            "int",        "literal",    3),
            LexMapLexer(identifiers,       "identifier", "identifier", 3)};

        lexer_set.push_back(LexMapLexer(startswith(grammar.comment_delimiter), "comment", "comment", 3));
        for (auto delimiter : grammar.string_delimiters)
        {
            lexer_set.push_back(LexMapLexer(startswith(string(1, delimiter)), "string", "literal", 1));
        }

        const vector<Seperator> whitespace = readWhitespaceFile(directory + "whitespace");
        LexMap test_language(term_sets, lexer_set, whitespace);
        grammar.lexmap = test_language;
        print("Done");
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
     * High level function for transpilation
     * Converts source files of one language to source files of another, copying them into a new directory
     * @param filenames   List of files to transpile
     * @param input_dir   Input directory containing files in input language
     * @param input_lang  String name of input langauge
     * @param output_dir  Output directory that will contain files in output language
     * @param output_lang String name of output language
     */ 
    void compileFiles(vector<string> filenames, string input_dir, string input_lang, string output_dir, string output_lang)
    {
        auto grammar   = loadGrammar(input_lang);
        auto generator = loadGenerator(output_lang);

        auto symbol_table = readSymbolTable("languages/symboltables/" + input_lang + output_lang);

        for (auto& file : filenames)
        {
            compile(file, grammar, generator, symbol_table, input_dir, output_dir);
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
     */
    void compile(string filename, Grammar& grammar, Generator& generator, unordered_map<string, string>& symbol_table, string input_directory, string output_directory)
    {
        print("Reading File");
        auto content         = readFile     (input_directory + "/" + filename);
        print("Lexing terms");
        auto tokens          = tokenPass    (content, grammar, symbol_table); 
        print("Creating symbols");
        auto symbolic_tokens = symbolicPass (tokens);
        print("Joining symbolic tokens");
        auto joined_tokens   = join         (symbolic_tokens, grammar.lexmap.newline);

        for(auto& jt : joined_tokens)
        {
            print("Joined Token: " + jt.type + ", " + jt.sub_type + ", \"" + jt.text + "\"");
        }

        print("Constructing from grammar:");
        unordered_map<string, tuple<vector<string>, string>> files;

        auto identified_groups = grammar.identifyGroups(joined_tokens);
        for (auto identified_group : identified_groups)
        {
            print("Compiling groups (Identified as " + get<0>(identified_group) + ")");
            string gen_with = "none";
            if (files.empty())
            {
                gen_with = filename;
            }
            unordered_set<string> names;
            auto groups = get<1>(identified_group);
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    print(symbol->abstract());
                }
            }
            auto generated = generator(names, get<1>(identified_group), get<0>(identified_group), gen_with);
            for (auto fileinfo : generated)
            {
                string type         = get<0>(fileinfo);
                string path         = get<1>(fileinfo);
                vector<string> body = get<2>(fileinfo);

                if (contains(files, type))
                {
                    print("Adding to " + type + " file");
                    concat(get<0>(files[type]), body);
                }
                else
                {
                    print("Creating initial " + type + " file");
                    files[type] = make_tuple(body, path);
                }
            }
        }

        print("Initial file");
        for (auto line : content)
        {
            print(line);
        }

        for (auto kv : files)
        {
            print("Generated " + kv.first + " file:");
            auto body = get<0>(kv.second);
            auto path = get<1>(kv.second);
            for (auto line : body)
            {
                print(line);
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
    std::vector<Tokens> tokenPass(std::vector<std::string> content, Grammar& grammar, unordered_map<string, string>& symbol_table)
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
                for (auto line : lines)
                {
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
                    print("Token Value: " + value);
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
    std::vector<vector<SymbolicToken>> symbolicPass(std::vector<Tokens> tokens)
    {
        std::vector<vector<SymbolicToken>> symbolic_tokens;
        for (auto token_group : tokens)
        {
            symbolic_tokens.push_back(toSymbolic(generatorMap, token_group));
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

}
