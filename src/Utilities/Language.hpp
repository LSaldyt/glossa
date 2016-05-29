#pragma once
#include "Lex/Seperate.hpp"

namespace Language
{
    using namespace Lex;
    struct Language
    {
        Seperators    seperators;

        Terms         keywords;
        Terms         operators;

        std::vector<std::tuple<ParseFunction, std::string>> parsers;

        ParseFunction number_parser;
        ParseFunction identifier_parser;
        ParseFunction keyword_parser;
        ParseFunction operator_parser;

        Language(const ParseFunction& set_number_parser     = digits,
                 const ParseFunction& set_identifier_parser = alphas,
                 const Terms& set_keywords  = Terms(),
                 const Terms& set_operators = Terms())
        {
            keywords          = set_keywords;
            operators         = set_operators;

            parsers.push_back(std::make_tuple(set_number_parser,     "int"));
            parsers.push_back(std::make_tuple(set_identifier_parser, "identifier"));
            parsers.push_back(std::make_tuple(justFrom(keywords),    "keyword"));
            parsers.push_back(std::make_tuple(justFrom(operators),   "operator"));

            seperators.insert(seperators.end(), whitespace.begin(), whitespace.end());

            seperators.reserve(operators.size());
            for (auto op : operators)
            {
                seperators.push_back(std::make_tuple(op, true));
            }
        }

        std::tuple<Token, Terms> identify(Terms terms) const
        {
            for (auto parser : parsers)
            {
                auto result = std::get<0>(parser)(terms);
                if(result.result)
                {
                    return std::make_tuple(std::make_tuple(result.parsed[0], std::get<1>(parser)), result.remaining);
                }
            }
            return std::make_tuple(std::make_tuple("", "failure"), Terms());
        }
    };
}
