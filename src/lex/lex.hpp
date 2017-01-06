#pragma once
#include "import.hpp"
#include "seperate.hpp"
#include "lexmap.hpp"

namespace lex
{
    Tokens lexWith(const string& sentence, const LexMap& language, vector<char> string_delimiters, string comment_delimiter);
}
