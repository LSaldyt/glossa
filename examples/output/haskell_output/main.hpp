#include "../std/std.hpp"
template < typename T_x> auto doubleMe ( T_x x) { std::vector<Object> __func_args__({ x}); /* Pattern-matched overloads: */ /* Default body: */ return x  + x  ;}  
/*No Representation*/ 
template < typename T_x> auto doubleSmallNumber ( T_x x) { std::vector<Object> __func_args__({ x}); /* Pattern-matched overloads: */ /* Default body: */ if ( x  > 100  ) { return x  ;} return x  * 2  ;}  
/*No Representation*/ 
const auto a = std::list<Object>({ 1 ,2 ,3 })   ;  
/*No Representation*/ 
const auto b = range( 10 , 1 , 2 )   ;  
/*No Representation*/ 
const auto c = listComp( b  , std::function<Object(const Object&)>( [&](const auto& x ) { return 2  * x  ; } ) );   ;  
/*No Representation*/ 
const auto d = listComp( c  , std::function<Object(const Object&)>( [&](const auto& x ) { return x  ; } ) , std::function<bool(const Object&)>( [&](const auto& x ) { return x  > 10 ; } ) );   ;  
/*No Representation*/ 
/*No Representation*/ 
const auto t = std::make_tuple( 1 ,2 )   ;  
/*No Representation*/ 
/*No Representation*/ 
// No main declaration required  
