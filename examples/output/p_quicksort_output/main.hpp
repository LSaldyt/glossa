#include "../std/std.hpp"
template < 
typename T_array
> 
auto sort ( 
T_array array
) 
{ 
auto less = std::vector<Object>({ })    ;
auto equal = std::vector<Object>({ })    ;
auto greater = std::vector<Object>({ })    ;
if ( len ( array )   <= 1  ) { return array   ;} else { auto pivot = array [ 0 ]    ;
for (auto x : array  ) { if ( x  < pivot  ) { less . push_back ( x )   ;}  ;
if ( x  == pivot  ) { equal . push_back ( x )   ;}  ;
if ( x  > pivot  ) { greater . push_back ( x )   ;}  ;}  ;
return sort ( less )   + equal  + sort ( greater )    ;}  ;
} 
// No main definition required 
