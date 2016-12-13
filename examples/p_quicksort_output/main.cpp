#include "main.hpp"
// No source for non-main functions 
int main(int argc, char ** argv) 
{ 
print ( "Running main"s )  ;
auto l = sort ( std::vector<Object>({ 6 ,5 ,4 ,743 })  )    ;
assert( l  == std::vector<Object>({ 4 ,5 ,6 ,743 })   );  ;
} 
