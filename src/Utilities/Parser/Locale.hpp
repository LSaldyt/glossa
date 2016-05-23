#pragma once
#include <algorithm>

bool is_digits(const std::string &str);
bool is_alphas(const std::string &str);
bool is_puncts(const std::string &str);
bool is_uppers(const std::string &str);
bool is_lowers(const std::string &str);

namespace Parse
{
    const auto digits = singleTemplate(is_digits);
    const auto alphas = singleTemplate(is_alphas);
    const auto puncts = singleTemplate(is_puncts);
    const auto uppers = singleTemplate(is_uppers);
    const auto lowers = singleTemplate(is_lowers);
}
