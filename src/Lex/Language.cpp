#include "Language.hpp"

namespace Lex
{
    LanguageLexer::LanguageLexer(function<Result<string>(vector<string>)> set_match_function, string set_name, string set_type, int set_precedence)
    {
        match      = set_match_function;
        name       = set_name;
        type       = set_type;
        precedence = set_precedence;
    }

    Language::Language(const LanguageTermSets& set_term_sets, const LanguageLexers&  set_language_lexers) {
        language_term_sets = set_term_sets;

        // Always seperate by whitespace
        concat(seperators, whitespace);

        // Custom language lexers (like int, string, etc..)
        concat(language_lexers, set_language_lexers);

        // Add term sets (like operators) to a language's seperators
        for (auto term_set : language_term_sets)
        {
            for (auto term : get<0>(term_set))
            {
                language_lexers.push_back(LanguageLexer(just(term), term, get<1>(term_set), 1));
                if(get<1>(term_set) != "keyword") //seperating by keywords would make identifiers containing keywords impossible
                {
                    seperators.push_back(make_tuple(term, true)); // Keep seperators from term sets (ie operators)
                }
            }
        }

        // Enforce lexing precedence
        sortBy(language_lexers, [](auto &left, auto &right) {
                    return left.precedence < right.precedence;
                    });
        print("Language lexers (sorted by precedence): ");
        for (auto lexer : language_lexers)
        {
            print(lexer.name + " " + lexer.type);
        }
    }

    tuple<Token, Terms> Language::identify(Terms terms) const
    {
        // Return result of first lexer to match against remaining terms
        for (auto lexer : language_lexers)
        {
            auto result = lexer.match(terms);
            if(result.result)
            {
                print("Term identified as " + lexer.name);
                return make_tuple(Token(result.consumed, lexer.name, lexer.type), result.remaining);
            }
        }

        // Error handling
        print("Could not identify terms:");
        for (auto t : terms)
        {
            print("\"" + t + "\"");
        }
        return make_tuple(Token({}, "unidentified", "failure"), Terms());
    }
}
