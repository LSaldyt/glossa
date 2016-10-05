#pragma once
#include "Types.hpp"
#include "Import.hpp"

namespace Match 
{
    template <typename T>
    struct Consumed
    {
        bool  result;
        string annotation;
        vector<T> consumed;

        Consumed(bool set_result    = false,
                 vector<T> set_consumed = vector<T>(),
                 string set_annotation  = "none"
                )
        {
            result   = set_result;
            consumed = set_consumed;
            annotation = set_annotation;
        }
    };
}
