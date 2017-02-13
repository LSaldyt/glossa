#include "../syntax/symbols/export.hpp"
#include "transformer.hpp"

namespace transform 
{
ElementConstructor<shared_ptr<Symbol>> ec = [](unordered_set<string>& names,
              SymbolStorage& symbol_storage,
              string,
              vector<string>& definitions,
              int nesting,
              OutputManager logger){
        return make_shared<Symbol>(Symbol());
        };
ElementConstructorCreator<shared_ptr<Symbol>> ec_creator = 
                [](string s){
                return ec;
                };

Transformer::Transformer(vector<string> transformer_files, string directory)
{
    for (auto file : transformer_files)
    {
        auto content = readFile(directory + file);
        auto constructor = generateTransformConstructor<shared_ptr<Symbol>>(content,
                ec_creator
                );
        transformation_map[file] = constructor;
    }
}

}
