#pragma once
#include "import.hpp"
#include "seperate.hpp"
#include "lexmap.hpp"

/**
 * Set of functions for converting lines of source code into annotated tokens
 */
namespace lex
{
    Tokens lexWith(const string& sentence, const LexMap& language, vector<char> string_delimiters, string comment_delimiter);
}
