#pragma once
#include "Import.hpp"
#include "Seperate.hpp"
#include "Language.hpp"

namespace Lex
{
    Tokens lex(const std::string& sentence, const Language& language);
}
