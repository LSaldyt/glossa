#pragma once
#include "constructor.hpp"
#include "fileconstructor.hpp"

namespace gen
{
    SymbolStorageGenerator generateSymbolStorageGenerator(vector<string> content);
    ConditionEvaluator     generateConditionEvaluator(vector<string> terms);

}
