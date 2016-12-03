#include "main.hpp"
int main(int argc, char ** argv) 
{ 
auto l = std::vector<Object>({ })    ;
l = std::vector<Object>({ 3 ,15 ,1 ,7 ,2 ,4 ,5 ,3 ,12 })    ;
auto d = std::unordered_map<Object, Object>({ })    ;
d = std::unordered_map<Object, Object>({ { 1  , 2  } ,{ 3  , 4  } })    ;
print ( "Running Main"s )  ;
if ( true  ) { print ( "Beginning while loop"s )  ;
auto x = 10   ;
while ( x  > 0  ) { print ( x )  ;
x = x  - 1   ;}  ;
return 0   ;} else { return 1   ;}  ;
} 
