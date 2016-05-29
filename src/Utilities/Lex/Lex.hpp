#pragma once
#include "Language.hpp"
#include "Seperate.hpp"
#include "../Syntax/Token.hpp"

namespace Lex
{
    using namespace Syntax;
    Tokens lex(const std::string& sentence, const Language& language);
}
