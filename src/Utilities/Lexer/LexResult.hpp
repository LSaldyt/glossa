#include "../Syntax/Token.hpp"

namespace Lexer
{
    using namespace Syntax;
    using namespace Parse;

    struct LexResult
    {
        bool   result;
        Tokens lexed;
    };
}
