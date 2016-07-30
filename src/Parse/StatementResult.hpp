#include "../Syntax/Statements.hpp"

namespace Parse{

  using namespace Syntax;
  struct StatementResult
  {
      bool result;
      SymbolicTokens parsed;
      std::shared_ptr<Statement> statement;
      StatementResult(bool set_result, SymbolicTokens set_parsed,
                      std::shared_ptr<Statement> set_statement);
  };
}
