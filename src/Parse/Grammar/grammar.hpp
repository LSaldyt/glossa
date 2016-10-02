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

using GrammarMap = std::unordered_map<std::string, std::tuple<SymbolicTokenParsers, std::vector<int>>>; 

class Grammar
{
public:
    Grammar(std::vector<std::string> grammar_files, std::string directory);
    GrammarMap grammar_map; 

    std::tuple<std::string, std::vector<Result<SymbolicToken>>> identify (SymbolicTokens& tokens);

private:
    std::tuple<SymbolicTokenParsers, std::vector<int>> read(std::string filename);
    std::tuple<bool, std::vector<Result<SymbolicToken>>> evaluateGrammar(SymbolicTokenParsers parsers, SymbolicTokens& tokens);

    SymbolicTokenParsers readGrammarPairs(std::vector<std::string>& terms);
    SymbolicTokenParser  readGrammarTerms(std::vector<std::string>& terms);
    SymbolicTokenParser  retrieveGrammar(std::string filename); 
};

}
