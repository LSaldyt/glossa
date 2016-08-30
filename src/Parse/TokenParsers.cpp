#include "TokenParsers.hpp"

namespace Parse
{
    SymbolicTokenParser subTypeParser(std::string sub_type)
    {
        const auto comparator = [sub_type](SymbolicToken token)
        {
            return token.sub_type == sub_type
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    SymbolicTokenParser typeParser(std::string type)
    {
        const auto comparator = [type](SymbolicToken token)
        {
            return token.type == type
        };
        return singleTemplate<SymbolicToken>(comparator);
    }

    SymbolicTokenParser dualTypeParser(std::string type, std::string sub_type)
    {
        return allOf({typeParser(type), subTypeParser(sub_type)});
    }
}
