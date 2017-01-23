#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"
#include "read.hpp"

namespace gen 
{

using TransformerConditionEvaluator = function<bool(SymbolStorage&, const vector<string>&)>;
using Transformer = function<shared_ptr<Symbol>(shared_ptr<Symbol>)>;

/**
 * Class for manipulating AST in a particular language
 */
class TransformerMap 
{

public:
    TransformerMap(vector<string> grammar_files, string directory);
};

}
