#pragma once
#include "../gen/read.hpp"
#include "../grammar/grammar.hpp"

namespace transform 
{
using namespace gen;
using namespace grammar;

using TransformerConditionEvaluator = function<bool(SymbolStorage&, const vector<string>&)>;
using Transformer                   = function<SymbolMatrix(SymbolMatrix)>;

/**
 * Class for manipulating AST in a particular language
 */
class TransformerMap 
{

public:
    TransformerMap(vector<string> grammar_files, string directory);
    IdentifiedGroups operator()(IdentifiedGroups identified_groups);

private:
    vector<tuple<string, Transformer>> transformers;
};

}
