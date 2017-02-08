#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"

namespace gen
{
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator     generateConditionEvaluator(vector<string> terms);
    Branch<string>         generateBranch(vector<string> content, 
                                          SymbolStorageGenerator symbol_storage_generator, 
                                          ElementConstructorCreator<string> ec_creator);

    vector<tuple<string, Constructor<string>>> generateConstructor(vector<string> content, 
            vector<tuple<string, FileConstructor>> file_constructors,
            ElementConstructorCreator<string> ec_creator);

}
