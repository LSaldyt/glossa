#include "std/std.hpp"
int main(){
Function f:
	Arguments: (auto x)
	Body: FunctionCall print: (
	Arguments: (x))
	Returns: Expression: x * 8)
Assignment: (x = Expression: 5 * FunctionCall f: (
	Arguments: (42)))
Assignment: (y = Expression: 3 + 2)
}
