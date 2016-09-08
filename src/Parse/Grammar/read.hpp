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


std::tuple<bool, std::vector<SymbolicTokens>> run(SymbolicTokenParsers parsers, SymbolicTokens& tokens);
}
