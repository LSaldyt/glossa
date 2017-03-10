#include "sentinel.hpp"
namespace syntax
{
string SentinelSymbol::name() 
{
    return val;
}
string SentinelSymbol::abstract(int indent) 
{
    return tag;
}
SentinelSymbol::SentinelSymbol(){}
SentinelSymbol::SentinelSymbol(string set_tag, string set_val)
    : tag(set_tag),
    val(set_val)
{
}
}
