#include "main.hpp"
int main(int argc, char ** argv) 
{ 
print ( "Hello World"s )  ;
auto l = std::vector<Object>({ 1 ,2 ,3 })    ;
for (auto i : l  ) { print ( i )  ;}  ;
auto d = listComp( l  , std::function<Object(const Object&)>( [&](const auto& i ) { return 2  * i  ; } ) );    ;
for (auto i : d  ) { print ( i )  ;}  ;
} 
