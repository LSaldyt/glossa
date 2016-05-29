# Lexer

### The `Lex` function takes a `Language` definition, and a string, returning `Tokens`, which are essentially tagged strings, ex: `"2000" : "number"`

##### `Token` is officially implemented as `std::tuple<Term, Term>`, where `Term` is a synonym for `std::string`

### The `Language` constructor:

        Language(const LanguageTermSets& set_term_sets,
                 const LanguageParsers&  set_parsers)


        using LanguageTermSet  = std::tuple<Terms, std::string>;
        using LanguageTermSets = std::vector<LanguageTermSet>;
        using LanguageParser   = std::tuple<ParseFunction, std::string>;
        using LanguageParsers  = std::vector<LanguageParser>;


##### Based on this information, the lexer can tag `"number"`, `"identifier"`, `"keyword"`, and `"operator"`
##### A tokenParser can be constructed to parse a list of types (`Tokens`), ex: `tokenParser(inOrder(justFrom({"number", "operator", "number"})))`

### It would be a good idea to write a function that simplifies the above construction process, such as :

      const auto buildTokenParser = [](std::vector<std::string> strings){return tokenParser(inOrder(justFrom(strings)));};

### Some example Usage:

    using namespace Parse;
    using namespace Lexer;
    using namespace Syntax;

    auto file = readFile("test_file.txt");
    for (auto line : file)
    {
        Terms keywords  = {"if", "then", "else"};
        Terms operators = {"+", "-", "*", "/"};

        Language test_language(digits, alphas, keywords, operators);

        auto tokens = lex(line, test_language);

        auto parseFunctions = {just("number"), just("operator"), just("number")};
        auto parser = tokenParser(inOrder(parseFunctions));
        std::cout << parser(tokens).result << std::endl;
    }
