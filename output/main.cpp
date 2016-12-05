#include "main.hpp"
// No source for non-main functions 
int main(int argc, char ** argv) 
{ 
print ( "Running main"s )  ;
auto l = std::vector<Object>({ 3 ,2 ,12 ,9 ,4 ,68 ,17 })    ;
print ( "Unsorte"s )  ;
for (auto item : l  ) { print ( item )  ;}  ;
l = sort ( l )    ;
print ( "Sorted"s )  ;
for (auto item : l  ) { print ( item )  ;}  ;
} 
