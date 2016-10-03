#include "std/std.hpp"
int main(){
Function Identifier: f:
	Arguments: (Identifier: x)
	Body: FunctionCall Identifier: print: (
	Arguments: (Identifier: x))
	Returns: Expression: Identifier: x Operator: * Literal: 8)
Assignment: (Identifier: x = Expression: Literal: 5 Operator: * FunctionCall Identifier: f: (
	Arguments: (Literal: 42)))
Assignment: (Identifier: y = Expression: Literal: 3 Operator: + Literal: 2)
}
