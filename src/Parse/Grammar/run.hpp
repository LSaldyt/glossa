#pragma once

#include "../../IO/IO.hpp"
#include "../../Lex/Seperate.hpp"
#include "../../Syntax/Statements.hpp"
#include "../TokenParsers.hpp"

#include <string>
#include <tuple>

namespace Grammar
{

using namespace Parse;
using namespace Match;
using namespace Syntax;

using GrammarMap = std::unordered_map<std::string, std::tuple<SymbolicTokenParsers, std::vector<int>>>; 

std::tuple<std::string, std::vector<Result<SymbolicToken>>> run (GrammarMap grammar_map, SymbolicTokens& tokens);
std::tuple<bool, std::vector<Result<SymbolicToken>>> evaluateGrammar(SymbolicTokenParsers parsers, SymbolicTokens& tokens);
}
