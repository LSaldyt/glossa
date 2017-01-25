#pragma once
#include "conditional.hpp"

namespace transform 
{
// Alias for a specific manipulation of a symbol matrix
using Transformer = function<tuple<string, SymbolMatrix>(SymbolMatrix, SymbolStorage&)>;

/**
 * Class for manipulating AST in a particular language
 */
class TransformerMap 
{

public:
    TransformerMap(vector<string> grammar_files, string directory);
    void operator()(IdentifiedGroups& identified_groups);

private:
    vector<tuple<string, Transformer, SymbolStorageGenerator>> transformers;
    void readTransformerFile(string filename);
    Transformer readTransformer(vector<string>);
    void transform(string& tag, SymbolMatrix& matrix);
    void transformSymbol(shared_ptr<Symbol>& symbol);
};

}
