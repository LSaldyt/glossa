#include "../std/std.hpp"
/*No Representation*/ 
template < typename T_l> auto quicksort ( T_l l) { std::vector<Object> __func_args__({ l}); /* Pattern-matched overloads: */ if (__func_args__ == std::vector<Object>({ std::vector<Object>({ }) })) { return std::vector<Object>({ })    ; } /* Default body: */ return listComp( l  , std::function<Object(const Object&)>( [&](const auto& a ) { return a  ; } ) , std::function<bool(const Object&)>( [&](const auto& a ) { return a  <= l [ 0 ]  ; } ) )   + std::vector<Object>({ l [ 0 ]  })   + listComp( l  , std::function<Object(const Object&)>( [&](const auto& a ) { return a  ; } ) , std::function<bool(const Object&)>( [&](const auto& a ) { return a  > l [ 0 ]  ; } ) )   ;}  
/*No Representation*/ 
const auto a = std::vector<Object>({ 5 ,4 ,3 ,2 ,1 ,2 ,3 ,4 ,5 })   ;  
/*No Representation*/ 
const auto b = quicksort ( a )   ;  
/*No Representation*/ 
/*No Representation*/ 
 
/*No Representation*/ 
