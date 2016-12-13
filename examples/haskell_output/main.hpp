#include "../std/std.hpp"
template < 
typename T_x
> 
auto doubleMe ( 
T_x x
) 
{ 
return x  + x   ;
} 
template < 
typename T_x
> 
auto doubleSmallNumber ( 
T_x x
) 
{ 
if ( x  > 100  ) { return x  ;} return x  * 2   ;
} 
const auto a = std::list<decltype( 1  )>({ 1 ,2 ,3 })   ; 
const auto b = range( 10 , 1 , 2 )   ; 
const auto c = listComp( b  , std::function<Object(const Object&)>( [&](const auto& x ) { return 2  * x  ; } ) );   ; 
const auto d = listComp( c  , std::function<Object(const Object&)>( [&](const auto& x ) { return x  ; } ) , std::function<bool(const Object&)>( [&](const auto& x ) { return x  > 10 ; } ) );   ; 
// No main definition required 
