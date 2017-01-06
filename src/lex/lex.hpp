#pragma once
#include "import.hpp"
#include "seperate.hpp"
#include "language.hpp"

namespace lex
{
    Tokens lexWith(const vector<string>& sentence, const Language& language, vector<char> vector<string>_delimiters, vector<string> comment_delimiter);
}
