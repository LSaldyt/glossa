#include "../syntax/syntax.hpp"

SymbolicToken::SymbolicToken(std::shared_ptr<syntax::Symbol> set_value, std::string set_sub_type, std::string set_type)
{
    value    = set_value;
    sub_type = set_sub_type;
    type     = set_type;
}
