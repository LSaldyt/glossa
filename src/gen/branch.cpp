#include "branch.hpp"

namespace gen
{

Branch::Branch(){}
Branch::Branch(ConditionEvaluator set_condition_evaluator, vector<LineConstructor> set_line_constructors, vector<Branch> set_nested_branches)
    : condition_evaluator(set_condition_evaluator),
      line_constructors(set_line_constructors),
      nested_branches(set_nested_branches)
{

}


}
