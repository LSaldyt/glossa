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
    vector<vector<shared_ptr<Symbol>>> groups;
    string tag;
    MultiSymbol();
    MultiSymbol(string set_tag, vector<vector<shared_ptr<Symbol>>> set_groups);

    virtual string representation(Generator& generator, unordered_set<string>& names, string filetype, int nesting=0);
    virtual string abstract(int indent=0);

    virtual tuple<string, vector<vector<shared_ptr<Symbol>>>> to_id_group();
    virtual void modify_id_group(string, vector<vector<shared_ptr<Symbol>>>);
};

}
