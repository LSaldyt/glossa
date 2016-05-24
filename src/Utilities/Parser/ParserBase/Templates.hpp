#pragma once
#include "../Types/Result.hpp" //NamedResult will automatically include its subclasses (Consumed and Result)
//Types/Types.hpp is also automatically included, since the types it defines are used to build the Result types

namespace Parse
{
    using ParseFunction   = std::function<Result(Terms)>;
    using ParseFunctions  = std::vector<ParseFunction>;
    using Consumer        = std::function<Consumed(Terms)>;

    ParseFunction parseTemplate(Consumer consumer);
    ParseFunction singleTemplate(Comparator comparator);
    ParseFunction multiTemplate(Consumer consumer);
}
