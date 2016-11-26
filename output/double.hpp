#include "../std/std.hpp"
template < 
typename T_x
> 
auto doubleMe ( 
T_x x
) 
{ 
return x   + x    ;   ;  
} 
template < 
typename T_x
> 
auto doubleSmallNumber ( 
T_x x
) 
{ 
if ( x    > 100     )  {  return x    ;  ; }  else  {  return x   * 2    ;  ; }   ;  
} 
// No main definition required 
