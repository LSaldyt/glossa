#include "../std/std.hpp"
        template <        typename T_array        >    auto sort (    T_array array    )    {            auto less = std::vector<Object>({})  ;
        auto equal = std::vector<Object>({})  ;
        auto greater = std::vector<Object>({})  ;
    if ( len (array) <= 1 )    {        return array  ;    }    else{        auto pivot = array[0]  ;
for (auto x : array){    if ( x < pivot )    {        less.push_back (x) ;    }     ;
    if ( x == pivot )    {        equal.push_back (x) ;    }     ;
    if ( x > pivot )    {        greater.push_back (x) ;    }     ;} ;
return sort (less) + equal + sort (greater)  ;} ;    } 
    auto __py_main__ (        )    {            auto l = sort (std::vector<Object>({3,2,12,9,4,68,17,1,2,3,4,5,6,12,9,8,7,6,5,4,743}))  ;    } 
 
