#pragma once
#include "import.hpp"
#include "seperate.hpp"
#include "language.hpp"

namespace Lex
{
    Tokens lex(const string& sentence, const Language& language);
}
