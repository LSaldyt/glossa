/// Copyright 2017 Lucas Saldyt
#pragma once
#include "import.hpp"
#include <algorithm>

/**
 * Set of parsers used when lexing/annotating inputs
 */

bool is_digits(const std::string &str);
bool is_alphas(const std::string &str);
bool is_puncts(const std::string &str);
bool is_uppers(const std::string &str);
bool is_lowers(const std::string &str);
bool is_identifiers(const std::string &str);
bool is_double(const std::string &str);
