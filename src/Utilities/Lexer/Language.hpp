#pragma once
#include "../Parser/Types/Types.hpp"
#include "../Parser/ParserBase/Seperate.hpp"


namespace Lexer
{
    using namespace Parse;

    struct Language
    {
        Seperators    seperators;

        Terms         keywords;
        Terms         operators;

        ParseFunction number;
        ParseFunction identifier;

        Language(const ParseFunction& set_number = digits,
                 const ParseFunction& set_identifier = alphas,
                 const Terms& set_keywords  = Terms(),
                 const Terms& set_operators = Terms())
        {
            number     = set_number;
            identifier = set_identifier;
            keywords   = set_keywords;
            operators  = set_operators;

            seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

            seperators.reserve(operators.size());
            for (auto op : operators)
            {
                seperators.push_back(std::make_tuple(op, true));
            }
        }
    };
}
