#include "Generator.hpp"

namespace Gen
{
    Generator::Generator(){}

    Generator::Generator(const SymbolicTokenParser &set_parser,
                         const StatementGenerator  &set_statement_generator)
    {
        parser              = set_parser;
        statement_generator = set_statement_generator;
    }

    std::tuple<std::string, bool> Generator::generate(SymbolicTokens tokens)
    {
        auto result = parser(tokens);
        if(result.result)
        {
            auto output = statement_generator(result.parsed)->generator();
            return std::make_tuple(output, true);
        }
        else
        {
            return std::make_tuple("", false);
        }
    }
}
