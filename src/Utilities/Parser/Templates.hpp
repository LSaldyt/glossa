#pragma once
#include "Types/NamedResult.hpp" //NamedResult will automatically include its subclasses (Consumed and Result)
//Types/Types.hpp is also automatically included, since the types it defines are used to build the Result types


namespace Parse
{
    using ParseFunction   = std::function<Result(Tokens)>;
    using ParseFunctions  = std::vector<ParseFunction>;
    using Consumer        = std::function<Consumed(Tokens)>;

    ParseFunction parseTemplate(Consumer consumer);
    ParseFunction singleTemplate(TokenComparator comparator);
    ParseFunction multiTemplate(Consumer consumer);
}
