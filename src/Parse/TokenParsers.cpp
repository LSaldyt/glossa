#include <exception>
#include <iostream>
#include "TokenParsers.hpp"

namespace Parse
{
    SymbolicTokenParser subTypeParser(std::string sub_type)
    {
        const auto comparator = [sub_type](SymbolicToken token)
        {
            return token.sub_type == sub_type;
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    SymbolicTokenParser typeParser(std::string type)
    {
        const auto comparator = [type](SymbolicToken token)
        {
            return token.type == type;
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    SymbolicTokenParser dualTypeParser(std::string type, std::string sub_type)
    {
        return allOf<SymbolicToken>({typeParser(type), subTypeParser(sub_type)});
    }

    std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)>
    discard
    (std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)> matcher)
    {
        using Syntax::Symbol;
        return [matcher](std::vector<SymbolicToken> terms)
        {
            auto result = matcher(terms);
            for (auto& term : result.consumed)
            {
                term = SymbolicToken(std::make_shared<Symbol>(Symbol()), "discard", "discard");
            }
            return result;
        };
    }

    std::vector<SymbolicToken>
    clean
    (const std::vector<SymbolicToken>& tokens)
    {
        std::vector<SymbolicToken> clean_tokens;
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

    // Version of many for seperating nested multi-token parsers. Unnestable
    std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)>
    manySeperated
    (std::function<Result<SymbolicToken>(std::vector<SymbolicToken>)> matcher)
    {
        using Syntax::Symbol;
        return [matcher](std::vector<SymbolicToken> terms)
        {
            auto consumed = std::vector<SymbolicToken>(); 
            std::string annotation = "none";

            while(terms.size() > 0)
            {
                auto result = matcher(terms);
                if (result.result)
                {
                    annotation = result.annotation;
                    terms = result.remaining;
                    if (not consumed.empty())
                    {
                        consumed.push_back(SymbolicToken(std::make_shared<Symbol>(Symbol()), "seperator", "seperator"));
                    }
                    consumed.insert(consumed.end(), result.consumed.begin(), result.consumed.end());
                }
                else
                {
                    break;
                }
            }

            auto result = Result<SymbolicToken>(true, consumed, terms);
            result.annotation = annotation;
            return result;
        };
    };

    std::vector<std::vector<SymbolicToken>>
    reSeperate
    (const std::vector<SymbolicToken>& tokens)
    {
        std::vector<std::vector<SymbolicToken>> grouped_tokens;

        auto it = tokens.begin();
        while (it != tokens.end())
        {
            if (it->type == "seperator")
            {
                if (it + 1!= tokens.end())
                {
                    grouped_tokens.push_back(std::vector<SymbolicToken>());
                }
            }
            else 
            {
                if (grouped_tokens.empty())
                {
                    grouped_tokens.push_back(std::vector<SymbolicToken>());
                }
                grouped_tokens.back().push_back(*it);
            }
            ++it;
        }

        return grouped_tokens;
    }
}
