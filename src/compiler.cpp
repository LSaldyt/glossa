#include "compiler.hpp"


int main(int argc, char* argv[])
{
    using namespace compiler;

    auto grammar_files = readFile("languages/python/grammar/core");
    auto grammar = Grammar(grammar_files, "languages/python/grammar/");

    auto constructor_files = readFile("languages/cpp/constructors/core");
    auto generator = Generator(constructor_files, "languages/cpp/constructors/");

    auto operators        = readFile("languages/python/grammar/operators");
    auto logicaloperators = readFile("languages/python/grammar/logicaloperators"); 
    auto punctuators      = readFile("languages/python/grammar/punctuators");

    LanguageTermSets term_sets;
    term_sets.push_back(make_tuple(grammar.keywords,  "keyword"));
    term_sets.push_back(make_tuple(logicaloperators, "logicaloperator"));
    term_sets.push_back(make_tuple(operators, "operator"));
    term_sets.push_back(make_tuple(punctuators, "punctuator"));

    LanguageLexers lexer_set = {
        LanguageLexer(digits, "int", "literal", 3),
        LanguageLexer(startswith("\""), "string", "literal", 1),
        LanguageLexer(identifiers, "identifier", "identifier", 3)};

    Language test_language(term_sets, lexer_set);
    grammar.language = test_language;

    vector<string> files;
    string input_directory  = "";
    string output_directory = "";

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            files.push_back(argv[i]);
        }
    }
    for (auto& file : files)
    {
        compile(file, grammar, generator, "input", "output");
    }
    print("Compilation finished");
}

namespace compiler
{
    void compile(string filename, Grammar& grammar, Generator& generator, string input_directory, string output_directory)
    {
        print("Reading File");
        auto content         = readFile     (input_directory + "/" + filename);
        print("Lexing terms");
        auto tokens          = tokenPass    (content, grammar.language); 
        print("Creating symbols");
        auto symbolic_tokens = symbolicPass (tokens);
        print("Joining symbolic tokens");
        auto joined_tokens   = join         (symbolic_tokens);

        for(auto jt : joined_tokens)
        {
            print("Joined Token: " + jt.type + ", " + jt.sub_type + ", " + jt.text);
        }

        print("Constructing from grammar:");
        vector<string> header;
        vector<string> source;
        unordered_set<string> names;

        header.push_back("#include \"../std/std.hpp\"");
        source.push_back("#include \"" + filename + ".hpp\"");

        auto identified_groups = grammar.identifyGroups(joined_tokens);
        for (auto identified_group : identified_groups)
        {
            print(get<0>(identified_group));
            auto generated = generator(names, get<1>(identified_group), get<0>(identified_group));
            concat(header, get<0>(generated));
            concat(source, get<1>(generated));
        }

        print("Header:");
        for (auto line : header)
        {
            print(line);
        }
        print("Source:");
        for (auto line : source)
        {
            print(line);
        }

        writeFile(source, output_directory + "/" + filename + ".cpp");
        writeFile(header, output_directory + "/" + filename + ".hpp");
    }

    std::vector<Tokens> tokenPass(std::vector<std::string> content, const Language& language)
    {
        std::vector<Tokens> tokens;
        for (auto line : content)
        {
            print("Lexing: " + line);
            tokens.push_back(lexWith(line, language));
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

    SymbolicTokens join(std::vector<SymbolicTokens> token_groups)
    {
        auto tokens = SymbolicTokens();
        for (auto token_group : token_groups)
        {
            for (auto t : token_group)
            {
                tokens.push_back(t);
            }
        }
        return tokens;
    }
}
