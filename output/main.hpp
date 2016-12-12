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
const auto l = std::list<decltype( 1  )>({ 1 ,2 ,3 })   ; 
const auto r = range( 10 , 1 , 2 )   ; 
// No main definition required 
