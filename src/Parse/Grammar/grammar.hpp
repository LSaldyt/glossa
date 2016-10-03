#pragma once
#include <tuple>
#include <vector>
#include <string>
#include <unordered_map>

#include "../../IO/IO.hpp"
#include "../../Lex/Seperate.hpp"
#include "../../Syntax/Statements.hpp"
#include "../TokenParsers.hpp"


namespace Grammar
{

using namespace Parse;
using namespace Match;
using namespace Syntax;

using StatementConstructor = std::function<std::shared_ptr<Statement>(std::vector<std::shared_ptr<Symbol>>)>;

using GrammarMap = std::unordered_map<std::string, std::tuple<SymbolicTokenParsers, std::vector<int>>>; 

std::vector<std::shared_ptr<Symbol>> fromTokens(std::vector<SymbolicToken>);

class Grammar
{
    const static std::unordered_map<std::string, StatementConstructor> construction_map;

public:
    Grammar(std::vector<std::string> grammar_files, std::string directory);

    std::vector<std::shared_ptr<Symbol>> constructFrom(SymbolicTokens& tokens);

private:
    GrammarMap grammar_map; 

    std::shared_ptr<Symbol> build(std::string name, std::vector<std::shared_ptr<Symbol>> symbols);

    std::tuple<std::string, std::vector<Result<SymbolicToken>>> identify (SymbolicTokens& tokens);
    std::shared_ptr<Symbol> construct(std::string name, std::vector<Result<SymbolicToken>> results);

    std::tuple<SymbolicTokenParsers, std::vector<int>> read(std::string filename);
    std::tuple<bool, std::vector<Result<SymbolicToken>>> evaluateGrammar(SymbolicTokenParsers parsers, SymbolicTokens& tokens);

    SymbolicTokenParsers readGrammarPairs(std::vector<std::string>& terms);
    SymbolicTokenParser  readGrammarTerms(std::vector<std::string>& terms);
    SymbolicTokenParser  retrieveGrammar(std::string filename); 
};

}
