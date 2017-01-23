#pragma once
#include "../gen/read.hpp"
#include "../grammar/grammar.hpp"

/**
 * Module for making transformations to an AST post-identification
 */
namespace transform 
{
using namespace gen;
using namespace grammar;

using Transformer = function<SymbolMatrix(SymbolMatrix, SymbolStorage&)>;

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
};

}
