#include "main.hpp"
int main(int argc, char ** argv) 
{ 
print ( "Hello World"s )  ;
auto l = range ( 10 )    ;
for (auto i : l  ) { print ( i )  ;}  ;
print ( "HERE"s )  ;
auto d = listComp( l  , std::function<Object(const Object&)>( [&](const auto& i ) { return 2  * i  ; } ) );    ;
for (auto i : d  ) { print ( i )  ;}  ;
auto e = listComp( d  , std::function<Object(const Object&)>( [&](const auto& i ) { return i  ; } ) , std::function<bool(const Object&)>( [&](const auto& i ) { return i  > 10 ; } ) );    ;
for (auto i : e  ) { print ( i )  ;}  ;
} 
