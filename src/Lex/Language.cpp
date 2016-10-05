#include "Language.hpp"

namespace Lex
{
    LanguageLexer::LanguageLexer(std::function<Result<std::string>(std::vector<std::string>)> set_match_function, std::string set_name, std::string set_type, int set_precedence)
    {
        match      = set_match_function;
        name       = set_name;
        type       = set_type;
        precedence = set_precedence;
    }

    Language::Language(const LanguageTermSets& set_term_sets, const LanguageLexers&  set_language_lexers) {
        language_term_sets = set_term_sets;

        // Always seperate by whitespace
        seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

        // Custom language lexers (like int, string, etc..)
        language_lexers.insert(language_lexers.end(), set_language_lexers.begin(), set_language_lexers.end());

        // Add term sets (like operators) to a language's seperators
        for (auto term_set : language_term_sets)
        {
            for (auto term : std::get<0>(term_set))
            {
                language_lexers.push_back(LanguageLexer(just(term), term, std::get<1>(term_set), 1));
                if(std::get<1>(term_set) != "keyword") //seperating by keywords would make identifiers containing keywords impossible
                {
                    seperators.push_back(std::make_tuple(term, true)); // Keep seperators from term sets (ie operators)
                }
            }
        }

        // Enforce lexing precedence
        std::sort(language_lexers.begin(), language_lexers.end(), [](auto &left, auto &right) {
                    return left.precedence < right.precedence;
                    });
        print("Language lexers (sorted by precedence): ");
        for (auto lexer : language_lexers)
        {
            print(lexer.name + " " + lexer.type);
        }
    }

    std::tuple<Token, Terms> Language::identify(Terms terms) const
    {
        // Return result of first lexer to match against remaining terms
        for (auto lexer : language_lexers)
        {
            auto result = lexer.match(terms);
            if(result.result)
            {
                print("Term identified as " + lexer.name);
                return std::make_tuple(Token(result.consumed, lexer.name, lexer.type), result.remaining);
            }
        }

        // Error handling
        print("Could not identify terms:");
        for (auto t : terms)
        {
            print("\"" + t + "\"");
        }
        return std::make_tuple(Token({}, "unidentified", "failure"), Terms());
    }
}
