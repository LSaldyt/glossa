/// Copyright 2017 Lucas Saldyt
#include "symbol.hpp"

namespace syntax
{

/**
 * Represents a user-defined type composed of several low-level syntax elements
 * i.e. Assignment: (Identifier, Expression)
 *                               Expression: (sep Operator Value)
 *                                                         Value: Literal, FunctionCall
 *                                                         ...
 */
struct MultiSymbol : public Symbol
{
    MultiSymbolTable table;
    string tag;
    MultiSymbol();
    MultiSymbol(string set_tag, MultiSymbolTable set_table);

    virtual string representation(Generator& generator, unordered_set<string>& names, string filetype, int nesting=0);
    virtual string abstract(int indent=0);
    virtual tuple<string, MultiSymbolTable> to_id_group();
    virtual void modify_id_group(string set_tag, MultiSymbolTable set_table);
};

}
