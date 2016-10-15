#include "locale.hpp"

bool is_digits(const std::string &str)
{
    return all_of(str.begin(), str.end(), ::isdigit); // C++11
}

bool is_alphas(const std::string &str)
{
    return all_of(str.begin(), str.end(), ::isalpha); // C++11
}

bool is_puncts(const std::string &str)
{
    return all_of(str.begin(), str.end(), ::ispunct); // C++11
}

bool is_uppers(const std::string &str)
{
    return all_of(str.begin(), str.end(), ::isupper); // C++11
}

bool is_lowers(const std::string &str)
{
    return all_of(str.begin(), str.end(), ::islower); // C++11
}
