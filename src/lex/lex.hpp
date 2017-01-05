#pragma once
#include "import.hpp"
#include "seperate.hpp"
#include "language.hpp"

namespace lex
{
    Tokens lexWith(const string& sentence, const Language& language, vector<char> string_delimiters, string comment_delimiter);
}
