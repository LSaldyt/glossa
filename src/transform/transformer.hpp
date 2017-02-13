#pragma once
#include "../gen/read.hpp"

namespace transform 
{
using namespace gen;

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
    Transformer(vector<string> transformer_files, string directory);
private:
    unordered_map<string, Constructor<shared_ptr<Symbol>>> transformation_map;
};

}
