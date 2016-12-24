#include "compiler.hpp"


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

    auto grammar   = loadGrammar(from);
    auto generator = loadGenerator(to);

    auto symbol_table = readSymbolTable("languages/symboltables/" + from + to);

    vector<string> files = slice(args, 2);
    string input_directory  = "";
    string output_directory = "";

    for (auto& file : files)
    {
        compile(file, grammar, generator, symbol_table, "input", "output");
    }
    print("Compilation finished");
}


namespace compiler
{

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

    Grammar loadGrammar(string language)
    {
        print("Loading grammar for " + language);
        auto grammar_files = readFile("languages/" + language + "/grammar/core");
        auto grammar       = Grammar(grammar_files, "languages/" + language + "/grammar/");

        auto operators        = readFile("languages/" + language + "/grammar/operators");
        auto logicaloperators = readFile("languages/" + language + "/grammar/logicaloperators"); 
        auto punctuators      = readFile("languages/" + language + "/grammar/punctuators");

        LanguageTermSets term_sets;
        term_sets.push_back(make_tuple(grammar.keywords, "keyword"));
        term_sets.push_back(make_tuple(logicaloperators, "logicaloperator"));
        term_sets.push_back(make_tuple(operators,        "operator"));
        term_sets.push_back(make_tuple(punctuators,      "punctuator"));

        LanguageLexers lexer_set = {
            LanguageLexer(just("    "s),     "tab",        "tab",        3),
            LanguageLexer(startswith("\t"s), "tab",        "tab",        3),
            LanguageLexer(digits,            "int",        "literal",    3),
            LanguageLexer(startswith("\""s), "string",     "literal",    1),
            LanguageLexer(identifiers,       "identifier", "identifier", 3)};

        const Seperators whitespace = readWhitespaceFile("languages/" + language + "/grammar/whitespace");
        Language test_language(term_sets, lexer_set, whitespace);
        grammar.language = test_language;
        print("Done");
        return grammar;
    }

    Generator loadGenerator(string language)
    {
        print("Loading constructors for " + language);
        auto constructor_files = readFile("languages/" + language + "/constructors/core");
        auto generator = Generator(constructor_files, "languages/" + language + "/constructors/");
        print("Done");
        return generator;
    }

    void compile(string filename, Grammar& grammar, Generator& generator, unordered_map<string, string>& symbol_table, string input_directory, string output_directory)
    {
        print("Reading File");
        auto content         = readFile     (input_directory + "/" + filename);
        print("Lexing terms");
        auto tokens          = tokenPass    (content, grammar, symbol_table); 
        print("Creating symbols");
        auto symbolic_tokens = symbolicPass (tokens);
        print("Joining symbolic tokens");
        auto joined_tokens   = join         (symbolic_tokens, grammar.language.newline);

        for(auto& jt : joined_tokens)
        {
            print("Joined Token: " + jt.type + ", " + jt.sub_type + ", \"" + jt.text + "\"");
        }

        print("Constructing from grammar:");
        unordered_map<string, tuple<vector<string>, string>> files;

        auto identified_groups = grammar.identifyGroups(joined_tokens);
        for (auto identified_group : identified_groups)
        {
            string gen_with = "none";
            if (files.empty())
            {
                gen_with = filename;
            }
            unordered_set<string> names;
            auto groups = get<1>(identified_group);
            print("Compiling groups (Identified as " + get<0>(identified_group) + ")");
            for (auto group : groups)
            {
                for (auto symbol : group)
                {
                    print(symbol->abstract());
                    print("...");
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

    std::vector<Tokens> tokenPass(std::vector<std::string> content, Grammar& grammar, unordered_map<string, string>& symbol_table)
    {
        std::vector<Tokens> tokens;
        for (auto line : content)
        {
            //print("Lexing: " + line);
            tokens.push_back(lexWith(line, grammar.language));
        }
        for (auto& token_group : tokens)
        {
            for (auto& token : token_group)
            {
                for (auto& value : token.values)
                {
                    if (contains(symbol_table, value))
                    {
                        value = symbol_table[value];
                    }
                }
            }
        }
        return tokens;
    }

    std::vector<SymbolicTokens> symbolicPass(std::vector<Tokens> tokens)
    {
        std::vector<SymbolicTokens> symbolic_tokens;
        for (auto token_group : tokens)
        {
            symbolic_tokens.push_back(toSymbolic(generatorMap, token_group));
        }
        return symbolic_tokens;
    }

    SymbolicTokens join(std::vector<SymbolicTokens> token_groups, bool newline)
    {
        auto tokens = SymbolicTokens();
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
