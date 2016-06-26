#include "Compiler.hpp"

namespace Compiler
{
    const auto identifierParser = typeParser(just("identifier"));
    const auto getRepr          = [](SymbolicTokens s){return s[0].value->representation();};

    const auto typeOrIdent = anyOf({just("type"), just("identifier")});
    const auto expression_parser = inOrder({
           typeOrIdent,
           many(inOrder({just("operator"), typeOrIdent}))
    });

    Expression buildExpression(SymbolicTokens& tokens)
    {
        Expression e;

        auto it = tokens.begin();
        if (it != tokens.end())
        {
            e.base = it->value;
        }
        it++;
        while(it != tokens.end())
        {
            if(it + 1 != tokens.end())
            {
                e.extensions.push_back(std::make_tuple(it->value, (it+1)->value));
                it++;
            }
            else
            {
                break;
            }
            it++;
        }
        tokens = SymbolicTokens(tokens.begin() + e.extensions.size() + 1, tokens.end());
    }

//    template < typename T >
//    std::function<std::tuple<bool, T>(SymbolicTokens)> from (std::function<TokenResult<SymbolicToken>(std::vector<SymbolicToken>)> function, std::function<T(SymbolicTokens)> converter)

//    template < typename T >
//    bool bind(T &t, std::function<std::tuple<bool, T>(SymbolicTokens)> typeGenerator, SymbolicTokens tokens)

    const auto genIdent = builder<std::string>(typeParser(just("identifier")), [](SymbolicTokens tokens){ return tokens[0].value->representation(); });

    StatementResult buildAssignment(SymbolicTokens& tokens)
    {
        Assignment a;
        auto result = bindTo<std::string>(a.identifier, genIdent, tokens);
        std::cout << result << std::endl;
    }

}
