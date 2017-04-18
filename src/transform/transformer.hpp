#pragma once
#include "../gen/read.hpp"
#include "../grammar/grammar.hpp"

namespace transform 
{

using namespace gen;
using namespace grammar;

using Register    = tuple<string, MultiSymbolTable>;
using RegisterMap = unordered_map<string, Register>; 

template <typename T>
Constructor<T> generateTransformConstructor(vector<string> content,
        ElementConstructorCreator<T>  ec_creator)
{
    auto constructors = generateConstructor<T>(
            content,
            {make_tuple("transform:", FileConstructor("none", {}))},
            ec_creator);
    assert(not constructors.empty());
    return get<1>(constructors[0]);
}

class Transformer
{
public:
    Transformer();
    Transformer(vector<string> transformer_files, string directory);
    void operator()(IdentifiedGroups& identified_groups);

private:
    unordered_map<string, Constructor<vector<string>>> transformation_map;
    void _transform(string& tag, MultiSymbolTable& ms_table);
    void _keyword_transform(vector<string>& terms, 
                            string& otag, 
                            MultiSymbolTable& oms_table,
                            RegisterMap& register_map);

};

}
