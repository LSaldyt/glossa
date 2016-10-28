#include "compiler.hpp"


int main(int argc, char* argv[])
{
    using namespace compiler;

    auto grammar = Grammar({"assignment", 
                            "expression", 
                            "value", 
                            "functioncall", 
                            "conditional", 
                            "boolexpression", 
                            "function",
                            "vector",
                            "dictionary", 
                            "tuple",
                            "forloop",
                            "whileloop",
                            "memberaccess",
                            "class",
                            "import",
                            "statement"}, "grammars/python/");

    auto operators        = readFile("grammars/python/operators");
    auto logicaloperators = readFile("grammars/python/logicaloperators"); 
    auto punctuators      = readFile("grammars/python/punctuators");

    LanguageTermSets term_sets;
    term_sets.push_back(make_tuple(grammar.keywords,  "keyword"));
    term_sets.push_back(make_tuple(logicaloperators, "logicaloperator"));
    term_sets.push_back(make_tuple(operators, "operator"));
    term_sets.push_back(make_tuple(punctuators, "punctuator"));

    LanguageLexers lexer_set = {
        LanguageLexer(digits, "int", "literal", 3),
        LanguageLexer(startswith("\""), "string", "literal", 1),
        LanguageLexer(alphas, "identifier", "identifier", 3)};

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
        compile(file, grammar, "input", "output");
    }
}

namespace compiler
{
    void compile(string filename, Grammar& grammar, string input_directory, string output_directory)
    {
        auto content         = readFile     (input_directory + "/" + filename);
        auto tokens          = tokenPass    (content, grammar.language); 
        auto symbolic_tokens = symbolicPass (tokens);
        auto joined_tokens   = join         (symbolic_tokens);

        for(auto jt : joined_tokens)
        {
            print("Joined Token: " + jt.type + ", " + jt.sub_type);
        }

        auto symbols = grammar.constructFrom(joined_tokens);

        print("\nAbstract syntax tree:\n");
        for (auto s : symbols)
        {
            print("Symbol annotated as \"" + s->annotation + "\" ");
            print(s->representation());
            print("");
        }

        auto files = generateFiles(filename, symbols);
        auto source = get<0>(files);
        auto header = get<1>(files);
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
