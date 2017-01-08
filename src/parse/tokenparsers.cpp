#include "tokenparsers.hpp"
#include "../syntax/symbols/export.hpp"

namespace parse
{
    /**
     * Parses a symbolictoken by its sub type
     * i.e. wildcard int 
     */
    SymbolicTokenParser subTypeParser(string sub_type)
    {
        const auto comparator = [sub_type](SymbolicToken token)
        {
            return token.sub_type == sub_type;
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    /**
     * Parses a symbolictoken by its type
     * i.e. operator wildcard 
     */
    SymbolicTokenParser typeParser(string type)
    {
        const auto comparator = [type](SymbolicToken token)
        {
            return token.type == type;
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    /**
     * Parses a symbolictoken by both its sub type and its type
     * i.e. operator +
     */
    SymbolicTokenParser dualTypeParser(string type, string sub_type)
    {
        return allOf<SymbolicToken>({typeParser(type), subTypeParser(sub_type)});
    }

    /**
     * Annotates all consumed tokens to be eventually discarded
     */
    SymbolicTokenParser
    discard
    (SymbolicTokenParser matcher)
    {
        using syntax::Symbol;
        return [matcher](vector<SymbolicToken> terms)
        {
            auto result = matcher(terms);
            for (auto& term : result.consumed)
            {
                //print("Discarding token: " + term.type + ", " + term.sub_type);
                term = SymbolicToken(make_shared<Symbol>(Symbol()), "discard", "discard", "");
            }
            return result;
        };
    }

    /**
     * Removes discard-annotated tokens from vector of tokens
     */
    vector<SymbolicToken>
    clean
    (const vector<SymbolicToken>& tokens)
    {
        vector<SymbolicToken> clean_tokens;
        clean_tokens.reserve(tokens.size());
        for (auto t : tokens)
        {
            if (t.type != "discard")
            {
                clean_tokens.push_back(t);
            }
        }
        return clean_tokens;
    }

    /**
     * Version of many for seperating nested multi-token parsers. Unnestable
     * Uses annotation to mark points between groups of consumed tokens
     */
    SymbolicTokenParser
    manySeperated
    (SymbolicTokenParser matcher, bool nonempty)
    {
        using syntax::Symbol;
        return [matcher, nonempty](vector<SymbolicToken> terms)
        {
            auto consumed = vector<SymbolicToken>(); 
            bool empty = true;

            while(terms.size() > 0)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    empty = false;
                    terms = result.remaining;
                    if (not consumed.empty())
                    {
                        consumed.push_back(SymbolicToken(make_shared<Symbol>(Symbol()), "seperator", "seperator", ""));
                    }
                    consumed.insert(consumed.end(), result.consumed.begin(), result.consumed.end());
                }
                else
                {
                    break;
                }
            }

            if (nonempty)
            {
                return Result<SymbolicToken>(!empty, consumed, terms);
            }
            else
            { 
                return Result<SymbolicToken>(true, consumed, terms);
            }
        };
    };

    /**
     * Turns annotated tokens into a 2D matrix of tokens
     */
    vector<vector<SymbolicToken>>
    reSeperate
    (const vector<SymbolicToken>& tokens)
    {
        vector<vector<SymbolicToken>> grouped_tokens;

        auto it = tokens.begin();
        while (it != tokens.end())
        {
            if (it->type == "seperator")
            {
                if (it + 1!= tokens.end())
                {
                    grouped_tokens.push_back(vector<SymbolicToken>());
                }
            }
            else 
            {
                if (grouped_tokens.empty())
                {
                    grouped_tokens.push_back(vector<SymbolicToken>());
                }
                grouped_tokens.back().push_back(*it);
            }
            ++it;
        }

        return grouped_tokens;
    }
}
