#include "../std/std.hpp"
template < 
typename T_x,typename T_y
> 
auto foo ( 
T_x x,T_y y
) 
{ 
return x   * y    ;   ;  
} 
template < 
typename T_name  ,typename T_sound  
> 
class 
__Animal__ 
{ 
public: 
T_name   name  ;
T_sound   sound  ;
__Animal__ 
( 
T_name   name  ,T_sound   sound  
) 
{ 
name  =name  ; sound  =sound  ;
} 

}; 
template < 
typename T_set_name,typename T_set_sound
> 
auto Animal ( 
T_set_name set_name,T_set_sound set_sound
){ 
print (  "Creating Animal"s    )   ;  ;
return 
__Animal__< 
decltype(set_name     ),decltype(set_sound     )
> 
( 
set_name     ,set_sound     
); 
} 
// No main definition required 
