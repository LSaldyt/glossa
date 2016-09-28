#include "read.hpp"
#include "grammar.hpp"

namespace Grammar
{

std::tuple<bool, std::vector<Result<SymbolicToken>>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens)
{
    std::vector<Result<SymbolicToken>> results;

    for (auto parser : parsers)
    {
        auto result = parser(tokens);
        if (result.result)
        {
            tokens = result.remaining;
            results.push_back(result);
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

    auto grammar = Grammar::Grammar({"assignment.grm", "expression.grm", "value.grm", "functioncall.grm"});

    using StatementConstructor = std::function<std::shared_ptr<Statement>(std::vector<SymbolicToken>)>;
    std::unordered_map<std::string, StatementConstructor> construction_map = {
        {
            "expression.grm", 
            [](std::vector<SymbolicToken> tokens){
                Expression e;
                e.base = tokens[0].value;
                if (tokens.size() > 1)
                {
                    if ((tokens.size() - 1) % 2 != 0)
                        throw std::exception();

                    for (int i = 1; i < tokens.size(); i += 2)
                    {
                        e.extensions.push_back(std::make_tuple(tokens[i].value, tokens[i + 1].value));
                    }
                }

                return std::make_shared<Expression>(e);
            }
        }
    };

    SymbolicTokens tokens = {SymbolicToken(std::make_shared<Identifier>(Identifier("x")), "identifier", "identifier"),
                             SymbolicToken(std::make_shared<Operator>(Operator("=")), "=", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(42)), "int", "literal"),
                             SymbolicToken(std::make_shared<Operator>(Operator("+")), "+", "operator"),
                             SymbolicToken(std::make_shared<Integer>(Integer(7)), "int", "literal"),
        };

    auto assignment_grammar = grammar.grammar_map["assignment.grm"];
    auto run_result = run(std::get<0>(assignment_grammar), tokens);
    if (std::get<0>(run_result))
    {
        std::cout << "Parsing successful, collecting results" << std::endl;
        
        std::vector<std::shared_ptr<Symbol>> result_symbols;

        auto construction_indices = std::get<1>(assignment_grammar);
        auto results = std::get<1>(run_result);
        for (auto i : construction_indices)
        {
            auto result = results[i];
            if (result.annotation == "none")
            {
                if (result.consumed.size() == 1)
                {
                    std::cout << result.consumed.back().value->representation() << std::endl; // ouch
                    result_symbols.push_back(result.consumed.back().value);
                }
                else
                {
                    throw std::exception();
                }
            }
            else
            {
                std::cout << construction_map[result.annotation](result.consumed)->representation() << std::endl;
                result_symbols.push_back(construction_map[result.annotation](result.consumed));
            }
        }

        Assignment a(result_symbols);
        std::cout << a.representation() << std::endl;

    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
}

