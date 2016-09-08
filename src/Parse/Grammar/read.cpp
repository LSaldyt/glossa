#include "read.hpp"
namespace Grammar
{

SymbolicTokenParsers read(std::string filename)
{
    SymbolicTokenParsers parsers;
    auto content = readFile(filename);
    
    for (auto line : content)
    {
        std::cout << line << std::endl;
        auto terms = Lex::seperate(line, {std::make_tuple(" ", false)});
        if (terms.size() == 1)
        {
            parsers.push_back(typeParser(terms[0]));
        }
        else
        {
            parsers.push_back(dualTypeParser(terms[0], terms[1]));
        }
    }
    return parsers;
}

std::tuple<bool, std::vector<SymbolicTokens>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<SymbolicTokens> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result.consumed);
        }
        else
        {
            return std::make_tuple(false, results);
        }
    }

    return std::make_tuple(true, results);
};

}

int main()
{
    using namespace Grammar;

    auto assignment = read("assignment.grm");
    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>(Operator("=")), "=", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(42)), "int", "type")};
    for (auto t : tokens)
    {
        std::cout << t.value->representation() << std::endl;
    }

    auto results = run(assignment, tokens);
    if (std::get<0>(results))
    {
        std::cout << "Parsed assignment from grammar file" << std::endl;
    }
}

