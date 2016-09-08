#pragma once
#include <vector>
#include <string>
#include "read.hpp"

namespace Grammar
{
class Grammar
{
public:
    Grammar(std::vector<std::string> grammar_files);
    std::unordered_map<std::string, SymbolicTokenParsers> grammar_map; 

private:

    SymbolicTokenParsers read(std::string filename);
};
}
