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
}
